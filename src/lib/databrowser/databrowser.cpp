#include "databrowser.h"
#include "databrowser_p.h"
#include "ui_databrowser.h"

#include <Widgetry/abstractdataeditfactory.h>
#include <Widgetry/abstractdataedit.h>
#include <Widgetry/widgetoperation.h>
#include <Widgetry/private/debug_p.h>

#include <DataGate/dataquery.h>
#include <DataGate/dataresponse.h>

#include <QtGui/qevent.h>

#include <QtWidgets/qmessagebox.h>
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

void DataBrowser::refresh()
{
    WIDGETRY_D(DataBrowser);

    if (!d->tableModel.manager()) {
        widgetryWarning() << "DataBrowser::refresh(): no data controller set !";
        return;
    }

    d->tableModel.get(newQuery(RefreshQuery));
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
        DataQuery query = newQuery(AddQuery);
        if (query.object().isEmpty())
            query.setObject(object);

        d->tableModel.manager()->addObject(query, d->progressCallback, [d, query](const DataResponse &response) {
            if (d->processDataResponse(query, response))
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
        DataQuery query = newQuery(EditQuery);
        if (query.object().isEmpty())
            query.setObject(object);

        d->tableModel.manager()->editObject(query, d->progressCallback, [d, query](const DataResponse &response) {
            if (d->processDataResponse(query, response))
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

    auto answer = QMessageBox::question(this, tr("Deletion"), tr("Do you really want to delete these %n item(s)", nullptr, objects.size()));
    if (answer != QMessageBox::Yes)
        return;

    DataQuery query = newQuery(DeleteQuery);
    if (query.array().isEmpty())
        query.setArray(ui->tableWidget->selectedObjects());

    d->tableModel.manager()->deleteObjects(query, [d, query, this](const DataResponse &response) {
        if (d->processDataResponse(query, response))
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

    ui->searchInput->setEnabled(d->hasDataFeature(AbstractDataManager::SearchByKeywords));

    ui->toggleFiltersButtons->setVisible(d->hasDataFeature(AbstractDataManager::SearchByFilters));
    if (ui->toggleFiltersButtons->isHidden())
        ui->filterFrame->setVisible(false);

    ui->tableWidget->view()->setSortingEnabled(d->hasDataFeature(AbstractDataManager::ResultsSorting));

    ui->addButton->setEnabled(d->hasDataFeature(AbstractDataManager::ObjectAdd));
    d->adaptToSelection();

    Widget::sync();
}

AbstractDataEdit *DataBrowser::filterEdit() const
{
    WIDGETRY_D(const DataBrowser);
    return d->filterWidget;
}

void DataBrowser::setFilterEdit(AbstractDataEdit *edit)
{
    WIDGETRY_D(DataBrowser);
    ui->filterContainerLayout->addWidget(edit->editWidget());
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

QVariant DataBrowser::handleOperation(const WidgetOperation &operation, bool *success)
{
    const QString function = operation.name();
    *success = true;

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

    return Widget::handleOperation(operation, success);
}

void DataBrowser::handleOperationResult(const WidgetOperation &operation, const QVariantHash &result, bool success)
{
    Widget::handleOperationResult(operation, result, success);
}

DataQuery DataBrowser::newQuery(QueryType type)
{
    WIDGETRY_D(DataBrowser);

    DataQuery query = AbstractDataClient::newQuery();

    switch (type) {
    case RefreshQuery:
        query = d->tableModel.dataQuery();
        query.setString(ui->searchInput->text());
        if (d->filterWidget)
            query.setFilters(d->filterWidget->object().toVariantHash());
        query.setClient(this);
        break;

    case ShowQuery:
        break;

    case AddQuery:
        break;

    case EditQuery:
        break;

    case DeleteQuery:
        break;

    case EmptyQuery:
        break;
    }

    return query;
}

DataQuery DataBrowser::newQuery()
{
    return newQuery(EmptyQuery);
}

bool DataBrowser::processDataResponse(const DataQuery &query, const DataResponse &response)
{
    Q_UNUSED(query);
    Q_UNUSED(response);
    return false;
}

void DataBrowser::showResponseMessage(const DataGate::DataQuery &query, const DataGate::DataResponse &response)
{
    QMessageBox box(this);
    box.setWindowFlag(Qt::CustomizeWindowHint, true);
    box.setWindowFlag(Qt::WindowStaysOnTopHint, true);

    if (response.isSuccess()) {
        box.setWindowTitle(response.title().isEmpty() ? tr("Message") : response.title());
        box.setIcon(QMessageBox::Information);
    } else {
        box.setWindowTitle(response.title().isEmpty() ? tr("Error") : response.title());
        box.setIcon(QMessageBox::Warning);
    }

    box.setText(response.text());
    box.setInformativeText(response.informativeText());
    box.setDetailedText(response.detailedText());

    box.exec();
}

DataBrowserPrivate::DataBrowserPrivate(DataBrowser *q, const QByteArray &id)
    : WidgetPrivate(q, id)
    , ui(nullptr)
    , editFactory(nullptr)
    , filterWidget(nullptr)
{
    progressCallback = [](qint64 current, qint64 total) {
        widgetryInfo() << "Request progress";
    };

    connect(&tableModel, &TableModel::finished, this, &DataBrowserPrivate::processModelResponse);
}

void DataBrowserPrivate::init(Ui::DataBrowser *ui)
{
    WIDGETRY_Q(DataBrowser);

    connect(ui->searchInput, &SearchBar::completionsRequested, this, &DataBrowserPrivate::fetchSearchSuggestions);

    ui->toggleFiltersButtons->hide();
    ui->filterFrame->hide();

    ui->tableWidget->addAction(ui->actionShow);
    ui->tableWidget->setModel(&tableModel);
    connect(ui->tableWidget, &TableWidget::selectionChanged, this, &DataBrowserPrivate::adaptToSelection);
    connect(ui->tableWidget, &TableWidget::doubleClicked, q, [q, this](const QModelIndex &index) { q->showItem(tableModel.object(index.row())); });

    QMenu *contextMenu = new QMenu(ui->tableWidget);
    contextMenu->addAction(ui->actionShow);
    contextMenu->addAction(ui->actionEdit);
    contextMenu->addAction(ui->actionDelete);
    ui->tableWidget->setContextMenu(contextMenu);

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

    DataQuery query = q->newQuery(DataBrowser::RefreshQuery);
    query.setString(text);
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

    DataQuery query = q->newQuery(DataBrowser::ShowQuery);
    query.setObject(item);
    controller->fetchObject(query, progressCallback, [=](const DataResponse &response) {
        if (processDataResponse(query, response) || !response.isSuccess())
            return;

        AbstractDataEdit *edit = editFactory->create(response.object(), operation, q_ptr);
        if (!edit) {
            widgetryWarning() << q->title() << ": unable to create an edit widget !";
            return;
        }

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
    if (response.isSuccess())
        adaptToSelection();
    processDataResponse(tableModel.dataQuery(), response);
}

bool DataBrowserPrivate::processDataResponse(const DataGate::DataQuery &query, const DataGate::DataResponse &response)
{
    WIDGETRY_Q(DataBrowser);
    if (response.hasMessage())
        q->showResponseMessage(query, response);
    return q->processDataResponse(query, response);
}

} // namespace Widgetry
