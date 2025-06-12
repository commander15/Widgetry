#include "databrowser.h"
#include "databrowser_p.h"
#include "ui_databrowser.h"

#include <Widgetry/application.h>
#include <Widgetry/private/databrowserhandler_p.h>
#include <Widgetry/abstractdataeditfactory.h>
#include <Widgetry/abstractdataedit.h>
#include <Widgetry/widgetoperation.h>
#include <Widgetry/private/debug_p.h>

#include <QtCore/qsettings.h>

#include <QtGui/qevent.h>

#include <DataGate/datarequest.h>
#include <DataGate/dataresponse.h>

#include <QtWidgets/qtableview.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qshortcut.h>

using namespace DataGate;

namespace Widgetry {

DataBrowser::DataBrowser(const QByteArray &id, QWidget *parent, Qt::WindowFlags flags)
    : Widget(new DataBrowserPrivate(this, id), parent, flags)
    , ui(new Ui::DataBrowser())
{
    WIDGETRY_D(DataBrowser);
    ui->setupUi(this);

    d->init(ui);

    auto registerHandler = [this, d](AbstractDataBrowserHandler *handler) {
        handler->browser = this;
        handler->ui = ui;
        handler->data = d;
        return handler;
    };

    auto interceptor = new RequestInterceptor();
    registerHandler(interceptor);
    d->requestInterceptors.append(interceptor);

    auto watcher = new RequestWatcher();
    registerHandler(watcher);
    d->requestWatchers.append(watcher);

    auto interaction = new InteractionHandler();
    registerHandler(interaction);
    d->interactionHandlers.append(interaction);

    setFocusProxy(ui->tableWidget);
}

DataBrowser::DataBrowser(QWidget *parent, Qt::WindowFlags flags)
    : Widget(QByteArray(), parent, flags)
{
}

DataBrowser::~DataBrowser()
{
    delete ui;
}

QVariant DataBrowser::parameter(const QString &name) const
{
    return property(name.toStdString().c_str());
}

void DataBrowser::setParameter(const QString &name, const QVariant &value)
{
    setProperty(name.toStdString().c_str(), value);
}

void DataBrowser::search(const QString &query)
{
    ui->searchInput->setText(query);
    refresh();
}

void DataBrowser::filter(const QVariantHash &filters)
{
    WIDGETRY_D(DataBrowser);
    if (d->filterWidget)
        d->filterWidget->setObject(Jsoner::Object::fromVariantHash(filters), d->filterWidget->operation());
    refresh();
}

void DataBrowser::reload()
{
    WIDGETRY_D(DataBrowser);
    d->tableModel.setPage(1);
    refresh();
}

void DataBrowser::refresh()
{
    WIDGETRY_D(DataBrowser);

    if (!d->tableModel.manager()) {
        widgetryWarning() << "DataBrowser::refresh(): no data controller set !";
        return;
    }

    d->beginRequest(d->tableModel.request());
    d->tableModel.get(newRequest(AbstractRequestInterceptor::IndexRequest));
}

Jsoner::Object DataBrowser::currentObject() const
{
    return ui->tableWidget->currentObject();
}

Jsoner::Array DataBrowser::selectedObjects() const
{
    return ui->tableWidget->selectedObjects();
}

void DataBrowser::showCurrentItem()
{
    showItem(ui->tableWidget->currentObject());
}

void DataBrowser::showItem(const Jsoner::Object &object)
{
    WIDGETRY_D(DataBrowser);
    d->openEdit(object, AbstractDataEdit::ShowOperation, nullptr);
}

void DataBrowser::addNewItem()
{
    addItem(Jsoner::Object());
}

void DataBrowser::addItem(const Jsoner::Object &object)
{
    WIDGETRY_D(DataBrowser);
    d->openEdit(object, AbstractDataEdit::AddOperation, [=](const Jsoner::Object &object) {
        DataRequest query = newRequest(AbstractRequestInterceptor::StoreRequest);
        if (query.object().isEmpty())
            query.setObject(object);

        d->beginRequest(query);
        d->tableModel.manager()->addObject(query, d->monitorRequest(query), [d, query](const DataResponse &response) {
            if (d->endRequest(query, response, false))
                return;

            if (response.isSuccess())
                QMetaObject::invokeMethod(d->q_ptr, "refresh", Qt::QueuedConnection);
        });
    });
}

void DataBrowser::editCurrentItem()
{
    editItem(ui->tableWidget->currentObject());
}

void DataBrowser::editItem(const Jsoner::Object &object)
{
    WIDGETRY_D(DataBrowser);
    d->openEdit(object, AbstractDataEdit::EditOperation, [=](const Jsoner::Object &object) {
        DataRequest query = newRequest(AbstractRequestInterceptor::UpdateRequest);
        if (query.object().isEmpty())
            query.setObject(object);

        d->tableModel.manager()->editObject(query, d->monitorRequest(query), [d, query](const DataResponse &response) {
            if (d->endRequest(query, response, false))
                return;

            if (response.isSuccess())
                QMetaObject::invokeMethod(d->q_ptr, "refresh", Qt::QueuedConnection);
        });
    });
}

void DataBrowser::deleteSelectedItems()
{
    deleteItems(ui->tableWidget->selectedObjects());
}

void DataBrowser::deleteItems(const Jsoner::Array &objects)
{
    WIDGETRY_D(DataBrowser);

    if (!d->tableModel.manager())
        return;

    QMessageBox::StandardButton answer = QMessageBox::No;
    for (AbstractInteractionHandler *handler : std::as_const(d->interactionHandlers)) {
        answer = handler->askDeletionConfirmation(objects);
        if (answer == QMessageBox::Yes)
            break;
    }

    if (answer != QMessageBox::Yes)
        return;

    DataRequest query = newRequest(AbstractRequestInterceptor::DestroyRequest);
    if (query.array().isEmpty())
        query.setArray(ui->tableWidget->selectedObjects());

    d->beginRequest(query);
    d->tableModel.manager()->deleteObjects(query, d->monitorRequest(query), [d, query, this](const DataResponse &response) {
        if (d->endRequest(query, response, false))
            return;

        if (response.isSuccess())
            refresh();
    });
}

QStringList DataBrowser::availableOperations() const
{
    static const QStringList operations = {
        "search", "filter", "refresh",
        "showCurrentItem", "addCurrentItem", "editCurrentItem", "deleteSelectedItems",
        "showItem", "addItem", "editItem", "deleteItems"
    };

    return Widget::availableOperations() + operations;
}

void DataBrowser::sync()
{
    WIDGETRY_D(DataBrowser);

    ui->searchInput->setVisible(d->hasDataFeature(AbstractDataManager::SearchByKeywords));
    ui->toggleFiltersButtons->setVisible(d->hasDataFeature(AbstractDataManager::SearchByFilters));
    ui->tableWidget->view()->setSortingEnabled(d->hasDataFeature(AbstractDataManager::ResultsSorting));
    ui->addButton->setEnabled(d->hasDataFeature(AbstractDataManager::ObjectAdd));

    d->adaptToSelection();
    Widget::sync();
}

void DataBrowser::loadSettings(QSettings *settings)
{
    settings->beginGroup("table");
    ui->tableWidget->loadSettings(settings);
    settings->endGroup();
}

void DataBrowser::saveSettings(QSettings *settings) const
{
    settings->beginGroup("table");
    ui->tableWidget->saveSettings(settings);
    settings->endGroup();
}

void DataBrowser::registerHandler(AbstractDataBrowserHandler *handler)
{
    WIDGETRY_D(DataBrowser);

    handler->browser = this;
    handler->ui = ui;
    handler->data = d;

    switch (handler->handlerType()) {
    case AbstractDataBrowserHandler::RequestInterceptor:
        if (!d->requestInterceptors.contains(handler))
            d->requestInterceptors.append(static_cast<AbstractRequestInterceptor *>(handler));
        break;

    case AbstractDataBrowserHandler::RequestWatcher:
        if (!d->requestWatchers.contains(handler))
            d->requestWatchers.insert(d->requestWatchers.size() - 2, static_cast<AbstractRequestWatcher *>(handler));
        break;

    case AbstractDataBrowserHandler::InteractionHandler:
        if (!d->interactionHandlers.contains(handler))
            d->interactionHandlers.insert(d->interactionHandlers.size() - 2, static_cast<AbstractInteractionHandler *>(handler));
        break;
    }
}

AbstractDataEdit *DataBrowser::filterEdit() const
{
    WIDGETRY_D(const DataBrowser);
    return d->filterWidget;
}

void DataBrowser::setFilterEdit(AbstractDataEdit *edit)
{
    WIDGETRY_D(DataBrowser);

    if (edit) {
        edit->setBrowser(this);

        QWidget *widget = edit->editWidget();
        widget->setContentsMargins(QMargins());
        ui->filterContainerLayout->addWidget(widget);
    }

    d->filterWidget = edit;
}

AbstractDataEditFactory *DataBrowser::editFactory() const
{
    WIDGETRY_D(const DataBrowser);
    return d->editFactory;
}

void DataBrowser::setEditFactory(AbstractDataEditFactory *factory)
{
    WIDGETRY_D(DataBrowser);
    d->editFactory = factory;
    sync();
}

AbstractDataManager *DataBrowser::dataManager() const
{
    WIDGETRY_D(const DataBrowser);
    return d->tableModel.manager();
}

void DataBrowser::setDataManager(DataGate::AbstractDataManager *manager)
{
    WIDGETRY_D(DataBrowser);
    d->tableModel.setManager(manager);
    sync();
}

DataEditFinishedCallback DataBrowser::editCallback(AbstractDataEdit::Operation operation)
{
    return [this, operation](const Jsoner::Object &object, int result) {
        if (object.isEmpty() || result != AbstractDataEdit::Accepted)
            return;

        switch (operation) {
        case AbstractDataEdit::ShowOperation:
            showItem(object);
            break;

        case AbstractDataEdit::AddOperation:
            addItem(object);
            break;

        case AbstractDataEdit::EditOperation:
            editItem(object);
            break;
        }
    };
}

void DataBrowser::prepareUi()
{
    WIDGETRY_D(DataBrowser);

    if (!d->tableModel.manager())
        setDataManager(Application::instance()->dataManager());

    if (d->tableModel.rowCount() == 0)
        refresh();
}

void DataBrowser::translateUi(bool full)
{
    WIDGETRY_D(DataBrowser);

    if (full)
        ui->retranslateUi(this);

    Widget::translateUi(full);
}

QVariant DataBrowser::handleOperationRequest(const WidgetOperation &operation, bool &success)
{
    const QString function = operation.name();
    success = true;

    if (function == QStringLiteral("search")) {
        search(operation.parameter().toString());
    } else if (function == QStringLiteral("filter")) {
        filter(operation.parameter().toHash());
    } else if (function == QStringLiteral("refresh")) {
        refresh();
    } else if (function == QStringLiteral("showCurrentItem")) {
        showCurrentItem();
    } else if (function == QStringLiteral("showItem")) {
        showItem(operation.parameter().toJsonObject());
    } else if (function == QStringLiteral("addNewItem")) {
        addNewItem();
    } else if (function == QStringLiteral("addItem")) {
        addItem(operation.parameter().toJsonObject());
    } else if (function == QStringLiteral("editCurrentItem")) {
        editCurrentItem();
    } else if (function == QStringLiteral("editItem")) {
        editItem(operation.parameter().toJsonObject());
    } else if (function == QStringLiteral("deleteSelectedItems")) {
        deleteSelectedItems();
    } else if (function == QStringLiteral("deleteItems")) {
        deleteItems(operation.parameter().toJsonArray());
    }

    return Widget::handleOperationRequest(operation, success);
}

void DataBrowser::handleOperationResult(const WidgetOperation &operation, const QVariantHash &result, bool success)
{
    Widget::handleOperationResult(operation, result, success);
}

DataRequest DataBrowser::newRequest(int type)
{
    WIDGETRY_D(DataBrowser);

    AbstractRequestInterceptor::RequestType requestType = AbstractRequestInterceptor::UnknownRequest;
    switch (type) {
    case AbstractRequestInterceptor::IndexRequest:
        requestType = AbstractRequestInterceptor::IndexRequest;
        break;

    case AbstractRequestInterceptor::ShowRequest:
        requestType = AbstractRequestInterceptor::ShowRequest;
        break;

    case AbstractRequestInterceptor::StoreRequest:
        requestType = AbstractRequestInterceptor::StoreRequest;
        break;

    case AbstractRequestInterceptor::UpdateRequest:
        requestType = AbstractRequestInterceptor::UpdateRequest;
        break;

    case AbstractRequestInterceptor::DestroyRequest:
        requestType = AbstractRequestInterceptor::DestroyRequest;
        break;

    case AbstractRequestInterceptor::UnknownRequest:
        break;
    }

    DataRequest request = AbstractDataClient::newRequest();
    for (AbstractRequestInterceptor *interceptor : std::as_const(d->requestInterceptors))
        interceptor->intercept(request, requestType);
    return request;
}

DataRequest DataBrowser::newRequest()
{
    return newRequest(AbstractRequestInterceptor::UnknownRequest);
}

DataBrowserPrivate::DataBrowserPrivate(DataBrowser *q, const QByteArray &id)
    : WidgetPrivate(q, id)
    , ui(nullptr)
    , editFactory(nullptr)
    , filterWidget(nullptr)
{
    connect(&tableModel, &TableModel::finished, this, &DataBrowserPrivate::processModelResponse);
}

DataBrowserPrivate::~DataBrowserPrivate()
{
    if (editFactory)
        delete editFactory;

    while (!requestInterceptors.isEmpty())
        delete requestInterceptors.takeFirst();

    while (!requestWatchers.isEmpty())
        delete requestWatchers.takeFirst();

    while (!interactionHandlers.isEmpty())
        delete interactionHandlers.takeFirst();
}

void DataBrowserPrivate::init(Ui::DataBrowser *ui)
{
    WIDGETRY_Q(DataBrowser);

    ui->iconButton->setIcon(q->icon());
    connect(q, &Widget::iconChanged, ui->iconButton, &QAbstractButton::setIcon);

    connect(ui->searchInput, &SearchBar::completionsRequested, this, &DataBrowserPrivate::fetchSearchSuggestions);

    ui->toggleFiltersButtons->installEventFilter(this);
    ui->filterFrame->installEventFilter(this);

    ui->tableWidget->addAction(ui->actionShow);
    ui->tableWidget->setModel(&tableModel);
    connect(ui->tableWidget, &TableWidget::fetchRequested, q, &DataBrowser::refresh);
    connect(ui->tableWidget, &TableWidget::selectionChanged, this, &DataBrowserPrivate::adaptToSelection);
    connect(ui->tableWidget, &TableWidget::doubleClicked, q, [q, this](const QModelIndex &index) { q->showItem(tableModel.object(index.row())); });

    QMenu *contextMenu = new QMenu(ui->tableWidget);
    contextMenu->addAction(ui->actionShow);
    contextMenu->addAction(ui->actionEdit);
    contextMenu->addAction(ui->actionDelete);
    ui->tableWidget->setContextMenu(contextMenu);

    ui->mainLayout->setContentsMargins(QMargins());

    QShortcut *shortcut = new QShortcut(QKeySequence::Find, q);
    connect(shortcut, &QShortcut::activated, ui->searchInput, QOverload<>::of(&QWidget::setFocus));

    this->ui = ui;
}

void DataBrowserPrivate::fetchSearchSuggestions(const QString &text)
{
    WIDGETRY_Q(DataBrowser);

    AbstractDataManager *manager = tableModel.manager();
    if (!manager || !manager->hasFeature(AbstractDataManager::SearchSuggestions, q))
        return;

    DataRequest query = q->newRequest(AbstractRequestInterceptor::IndexRequest);
    query.setQuery(text);
    manager->fetchSearchSuggestions(query, [this](const DataResponse &response) {
        if (!response.isSuccess())
            return;

        const Jsoner::Array suggestions = response.array();
        QStringList keywords;
        std::transform(suggestions.begin(), suggestions.end(), std::back_inserter(keywords), [](const QJsonValue &value) {
            return value.toString();
        });

        ui->searchInput->addCompletions(keywords);
    });
}

void DataBrowserPrivate::beginRequest(const DataGate::DataRequest &query)
{
    for (AbstractRequestWatcher *watcher : std::as_const(requestWatchers))
        if (watcher->requestStarted(query))
            return;
}

DataRequestCallback DataBrowserPrivate::monitorRequest(const DataGate::DataRequest &query)
{
    return [this, query](qint64 progress, qint64 total) {
        for (AbstractRequestWatcher *watcher : std::as_const(requestWatchers))
            watcher->requestRunning(query, progress, total);
    };
}

bool DataBrowserPrivate::endRequest(const DataRequest &query, const DataResponse &response, bool commit)
{
    WIDGETRY_Q(DataBrowser);

    if (!response.isSuccess() && filterWidget)
        filterWidget->setObject(QJsonObject::fromVariantMap(query.filters()));

    bool error = false;

    if (commit) {
        for (AbstractRequestWatcher *watcher : std::as_const(requestWatchers)) {
            if (!watcher->requestFinished(query, response)) {
                error = true;
                break;
            }
        }
    }

    if (response.hasMessage()) {
        for (AbstractInteractionHandler *handler : std::as_const(interactionHandlers))
            if (handler->showMessage(query, response))
                break;
    }

    return error;
}

void DataBrowserPrivate::openEdit(const QJsonObject &item, AbstractDataEdit::Operation operation, const EditingCallback &callback)
{
    WIDGETRY_Q(DataBrowser);

    if (!editFactory) {
        widgetryWarning() << q->title() << ": no edit factory set !";
        return;
    }

    AbstractDataManager *controller = tableModel.manager();

    if (!controller) {
        widgetryWarning() << q->title() << ": no data controller set !";
        return;
    }

    DataRequest query = q->newRequest(AbstractRequestInterceptor::ShowRequest);
    query.setObject(item);

    beginRequest(query);
    controller->fetchObject(query, monitorRequest(query), [=](const DataResponse &response) {
        if (endRequest(query, response) || !response.isSuccess())
            return;

        AbstractDataEdit *edit = editFactory->create(response.object(), operation, q_ptr);
        if (!edit) {
            widgetryWarning() << q->title() << ": unable to create an edit widget !";
            return;
        }

        edit->setBrowser(q);
        edit->exec([callback](const Jsoner::Object &item, int result) {
            if (result == AbstractDataEdit::Accepted && callback)
                callback(item);
        });
    });
}

bool DataBrowserPrivate::hasDataFeature(AbstractDataManager::Feature feature) const
{
    WIDGETRY_Q(DataBrowser);
    // ToDo: add optional (bool attrib based) permission check here !
    AbstractDataManager *dataController = tableModel.manager();
    return dataController ? dataController->hasFeature(feature, q) : false;
}

void DataBrowserPrivate::adaptToSelection()
{
    const QModelIndexList selected = ui->tableWidget->selectedRows();
    const bool single = selected.count() == 1;
    const bool multiple = selected.count() > 1;

    ui->editButton->setEnabled(single && hasDataFeature(AbstractDataManager::ObjectEdit));
    ui->deleteButton->setEnabled((single || multiple) && hasDataFeature(AbstractDataManager::ObjectDelete));

    ui->actionShow->setVisible(single);
    ui->actionEdit->setVisible(single && hasDataFeature(AbstractDataManager::ObjectEdit));
    ui->actionDelete->setVisible((single || multiple) && hasDataFeature(AbstractDataManager::ObjectDelete));
}

void DataBrowserPrivate::processModelResponse(const DataGate::DataResponse &response)
{
    if (response.isSuccess()) {
        ui->tableWidget->view()->scrollToTop();
        adaptToSelection();
    }

    endRequest(tableModel.request(), response);
}

bool DataBrowserPrivate::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->toggleFiltersButtons) {
        switch (event->type()) {
        case QEvent::EnabledChange:
            ui->filterFrame->setVisible(ui->toggleFiltersButtons->isChecked());
            break;

        case QEvent::Hide:
            ui->filterFrame->hide();
            break;

        default:
            break;
        }

        return QObject::eventFilter(watched, event);
    }

    if (watched == ui->filterFrame) {
        switch (event->type()) {
        case QEvent::Show:
            ui->mainLayout->setHorizontalSpacing(6);
            ui->toggleFiltersButtons->setChecked(true);
            break;

        case QEvent::Hide:
            ui->mainLayout->setHorizontalSpacing(0);
            ui->toggleFiltersButtons->setChecked(false);
            break;

        default:
            break;
        }

        return QObject::eventFilter(watched, event);
    }

    return QObject::eventFilter(watched, event);
}

} // namespace Widgetry
