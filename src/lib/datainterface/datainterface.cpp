#include "datainterface.h"
#include "datainterface_p.h"
#include "ui_datainterface.h"

#include <Widgetry/operation.h>
#include <Widgetry/abstractdatacontroller.h>
#include <Widgetry/dataquery.h>
#include <Widgetry/dataresponse.h>
#include <Widgetry/dataedit.h>
#include <Widgetry/abstractdataeditfactory.h>
#include <Widgetry/datatablemodel.h>

#include <Jsoner/object.h>

#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qmenu.h>

namespace Widgetry {

DataInterface::DataInterface(QWidget *parent, Qt::WindowFlags flags)
    : DataInterface(QByteArray(), parent, flags)
{
}

DataInterface::DataInterface(const QByteArray &id, QWidget *parent, Qt::WindowFlags flags)
    : UserInterface(new DataInterfacePrivate(this, id), parent, flags)
    , ui(new Ui::DataInterface())
{
    ui->setupUi(this);

    connect(ui->refreshButton, &QAbstractButton::clicked, this, &DataInterface::refresh);
    connect(ui->addButton, &QAbstractButton::clicked, this, &DataInterface::addNewItem);
    connect(ui->editButton, &QAbstractButton::clicked, this, &DataInterface::editCurrentItem);
    connect(ui->deleteButton, &QAbstractButton::clicked, this, &DataInterface::deleteSelectedItems);

    WIDGETRY_D(DataInterface);

    d->forge->init();

    d->addCallback = [this](const Jsoner::Object &object, int result) {
        if (object.isEmpty() || result != AbstractDataEdit::Accepted)
            return;

        executeDataRequest(&AbstractDataController::addObject, object, [this](const DataResponse &response) {
            if (response.isSuccess()) {
                refresh();
                return;
            }

            showResponseMessage(tr("Error during data download !"), response);
            return;
            int answer = QMessageBox::question(nullptr, tr("Operation failed"), tr("Do you want to retry ?"));
        });
    };

    d->editCallback = [this](const Jsoner::Object &object, int result) {
        if (object.isEmpty() || result != AbstractDataEdit::Accepted)
            return;

        executeDataRequest(&AbstractDataController::editObject, object, [this](const DataResponse &response) {
            if (response.isSuccess())
                refresh();
            else
                showResponseMessage(tr("Error during data saving !"), response);
        });
    };
}

DataInterface::~DataInterface()
{
    delete ui;
}

bool DataInterface::isOperationSupported(const QString &operation) const
{
    return s_availableOperations.contains(operation);
}

QStringList DataInterface::availableOperations() const
{
    return UserInterface::availableOperations() + s_availableOperations;
}

DataInterfaceForge *DataInterface::forge() const
{
    WIDGETRY_D(DataInterface);
    return d->forge;
}

AbstractDataController *DataInterface::dataController() const
{
    WIDGETRY_D(const DataInterface);
    return d->dataController;
}

void DataInterface::setDataController(AbstractDataController *controller)
{
    WIDGETRY_D(DataInterface);
    d->dataController = controller;
}

void DataInterface::search(const QString &query)
{
    ui->searchInput->setText(query);
    refresh();
}

void DataInterface::filter(const QVariantHash &filters)
{
    WIDGETRY_D(DataInterface);

    if (d->filterWidget) {
        d->filterWidget->setObject(Jsoner::Object::fromVariantHash(filters), AbstractDataEdit::EditOperation);
    }

    refresh();
}

void DataInterface::refresh()
{
    WIDGETRY_D(DataInterface);

    if (!d->dataController)
        return;

    DataQuery query;
    query.setQuery(ui->searchInput->text());
    if (d->filterWidget)
        query.setFilters(d->filterWidget->object().toVariantHash());
    if (!d->tableModel->sortField().isEmpty())
        query.setSort(d->tableModel->sortField(), d->tableModel->sortOrder());
    query.setPage(ui->pageInput->value() > 0 ? ui->pageInput->value() : 1);

    executeDataRequest(&AbstractDataController::fetchObjects, query, [this, d](const DataResponse &response) {
        if (!response.isSuccess()) {
            showResponseMessage(tr("Error during data download !"), response);
            return;
        }

        d->tableModel->setArray(response.objects());

        ui->pageInput->setMaximum(response.pageCount());
        ui->pageInput->setValue(response.page());
        ui->pageInput->setSuffix(" / " + QString::number(response.pageCount()));

        ui->previousPageButton->setEnabled(response.page() > 1);
        ui->nextPageButton->setEnabled(response.page() < response.pageCount());
    });
}

void DataInterface::showCurrentItem()
{
    showItem(currentObject());
}

void DataInterface::showItem(const Jsoner::Object &object)
{
    WIDGETRY_D(DataInterface);
    if (!d->dataEditFactory || object.isEmpty())
        return;

    preFetch(object, [this](const Jsoner::Object &object) { showObject(object); });
}

void DataInterface::addNewItem()
{
    addItem(Jsoner::Object());
}

void DataInterface::addItem(const Jsoner::Object &object)
{
    WIDGETRY_D(DataInterface);
    if (!d->dataEditFactory)
        return;

    addObject(object, d->addCallback);
}

void DataInterface::editCurrentItem()
{
    editItem(currentObject());
}

void DataInterface::editItem(const Jsoner::Object &object)
{
    WIDGETRY_D(DataInterface);
    if (!d->dataEditFactory || object.isEmpty())
        return;

    preFetch(object, [this, d](const Jsoner::Object &object) {
        editObject(object, d->editCallback);
    });
}

void DataInterface::deleteSelectedItems()
{
    deleteItems(selectedObjects());
}

void DataInterface::deleteItems(const Jsoner::Array &objects)
{
    WIDGETRY_D(DataInterface);
    if (objects.isEmpty())
        return;

    int response = QMessageBox::question(nullptr, tr("Deletion..."), tr("Do you really want to delete these %n record(s)", "", objects.size()));
    if (response == QMessageBox::No)
        return;

    executeDataRequest(&AbstractDataController::deleteObjects, objects, [this](const DataResponse &response) {
        if (response.isSuccess()) {
            refresh();
            return;
        }

        showResponseMessage(tr("Error during deletion !"), response);
    });
}

void DataInterface::prepareUi()
{
    WIDGETRY_D(DataInterface);
    if (d->tableModel && d->tableModel->rowCount() == 0)
        refresh();

    UserInterface::prepareUi();
}

void DataInterface::translateUi(bool full)
{
    WIDGETRY_D(DataInterface);

    if (full)
        ui->retranslateUi(this);

    if (d->showAction)
        d->showAction->setText(tr("Show"));

    if (d->editAction)
        d->editAction->setText(tr("Edit"));

    if (d->deleteAction)
        d->deleteAction->setText(tr("Delete"));

    UserInterface::translateUi();
}

Jsoner::Object DataInterface::currentObject() const
{
    WIDGETRY_D(const DataInterface);
    const int index = ui->tableView->currentIndex().row();
    return (index >= 0 ? d->tableModel->object(index) : Jsoner::Object());
}

Jsoner::Array DataInterface::selectedObjects() const
{
    WIDGETRY_D(const DataInterface);

    QItemSelectionModel *model = ui->tableView->selectionModel();
    const QModelIndexList indexes = (model ? model->selectedRows() : QModelIndexList());

    Jsoner::Array objects;
    for (const QModelIndex &index : indexes)
        objects.append(d->tableModel->object(index.row()));
    return objects;
}

void DataInterface::showObject(const Jsoner::Object &object)
{
    WIDGETRY_D(DataInterface);

    if (!d->dataEditFactory)
        return;

    AbstractDataEdit *edit = d->dataEditFactory->create(object, AbstractDataEdit::ShowOperation, this);
    if (edit) {
        edit->exec(nullptr);
    } else {
        const int count = d->dataEditFactory->editCount();
        showMaxWindowMessage(count, count);
    }
}

void DataInterface::addObject(const Jsoner::Object &object, const DataEditFinishedCallback &callback)
{
    WIDGETRY_D(DataInterface);

    if (!d->dataEditFactory)
        return;

    AbstractDataEdit *edit = d->dataEditFactory->create(object, AbstractDataEdit::AddOperation, this);
    if (edit) {
        edit->exec(nullptr);
    } else {
        const int count = d->dataEditFactory->editCount();
        showMaxWindowMessage(count, count);
    }
}

void DataInterface::editObject(const Jsoner::Object &object, const DataEditFinishedCallback &callback)
{
    WIDGETRY_D(DataInterface);

    if (!d->dataEditFactory)
        return;

    AbstractDataEdit *edit = d->dataEditFactory->create(object, AbstractDataEdit::EditOperation, this);
    if (edit) {
        edit->exec(nullptr);
    } else {
        const int count = d->dataEditFactory->editCount();
        showMaxWindowMessage(count, count);
    }
}

void DataInterface::showContextMenu(const Jsoner::Array &objects, const QPoint &pos)
{
    WIDGETRY_D(DataInterface);
    if (prepareContextMenu(objects, d->contextMenu))
        d->contextMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

bool DataInterface::prepareContextMenu(const Jsoner::Array &objects, QMenu *menu)
{
    WIDGETRY_D(DataInterface);
    return d->forge->prepareContextMenu(objects, menu);
}

void DataInterface::beginRequest()
{
}

void DataInterface::monitorRequest(int, int)
{
}

void DataInterface::endRequest()
{
}

bool DataInterface::handleOperation(Operation *operation)
{
    const QString name = operation->name();
    const QVariantHash parameters = operation->parameters();

    if (name == "search") {
        return handleSearch(parameters);
    }
    if (name == "filter") {
        return handleFilter(parameters);
    }
    if (name == "refresh") {
        return handleRefresh(parameters);
    }
    if (name == "showItem") {
        return handleShowItem(parameters);
    }
    if (name == "addItem") {
        return handleAddItem(parameters);
    }
    if (name == "editItem") {
        return handleEditItem(parameters);
    }
    if (name == "deleteItems") {
        return handleDeleteItems(parameters);
    }

    return false;
}
bool DataInterface::handleSearch(const QVariantHash &parameters)
{
    search(parameters.value("query").toString());
    return true;
}

bool DataInterface::handleFilter(const QVariantHash &parameters)
{
    filter(parameters.value("filters").toHash());
    return true;
}

bool DataInterface::handleRefresh(const QVariantHash &parameters)
{
    refresh();
    return true;
}

bool DataInterface::handleShowItem(const QVariantHash &parameters)
{
    showItem(parameters.value("item").toJsonObject());
    return true;
}

bool DataInterface::handleAddItem(const QVariantHash &parameters)
{
    addItem(parameters.value("item").toJsonObject());
    return true;
}

bool DataInterface::handleEditItem(const QVariantHash &parameters)
{
    editItem(parameters.value("item").toJsonObject());
    return true;
}

bool DataInterface::handleDeleteItems(const QVariantHash &parameters)
{
    deleteItems(parameters.value("items").toJsonArray());
    return true;
}

void DataInterface::handleOperationResult(const Operation &operation)
{
    Q_UNUSED(operation);
}

DataQuery DataInterface::prepareQuery(const DataQuery &query) const
{
    return query;
}

DataEditFinishedCallback DataInterface::editCallback(AbstractDataEdit::Operation operation) const
{
    WIDGETRY_D(DataInterface);
    switch (operation) {
    case AbstractDataEdit::AddOperation:
        return d->addCallback;

    case AbstractDataEdit::EditOperation:
        return d->editCallback;

    default:
        return nullptr;
    }
}

void DataInterface::showMaxWindowMessage(int activeEdits, int maxActiveEdits)
{
    QString message = QStringLiteral("It looks like you've opened the maximum number of windows on the '%1' interface. Close one to open a new one.");
    message.append("<br><br>Active windows count: <b>%2</b>.");

    QStringList details;
    details.append(QStringLiteral("Interface: ") + title());
    details.append(QStringLiteral("Max windows count: %1.").arg(maxActiveEdits));

    QMessageBox box;
    box.setTextFormat(Qt::RichText);
    box.setWindowTitle(tr("Too many windows open"));
    box.setText(message.arg(title()).arg(activeEdits));
    box.setDetailedText(details.join("\n"));
    box.exec();
}

void DataInterface::showResponseMessage(const QString &title, const DataResponse &response)
{
    showResponseMessage(title, QString(), response);
}

void DataInterface::showResponseMessage(const QString &title, const QString &text, const DataResponse &response)
{
    QMessageBox box;
    box.setWindowTitle(!title.isEmpty() ? title : (response.isSuccess() ? tr("Message") : tr("Error")));
    box.setIcon(response.isSuccess() ? QMessageBox::Information : QMessageBox::Warning);
    box.setText("<b>" + (!text.isEmpty() ? text : response.text()) + "</b>");
    box.setInformativeText(response.informativeText());
    box.setDetailedText(response.detailedText());
    box.exec();
}

void DataInterface::preFetch(const DataQuery &query, const std::function<void (const Jsoner::Object &)> &callback)
{
    executeDataRequest(&AbstractDataController::fetchObject, query, [this, callback](const DataResponse &response) {
        if (!response.isSuccess()) {
            showResponseMessage(tr("Error during data download !"), response);
            return;
        }

        callback(response.object());
    });
}

void DataInterface::executeDataRequest(DataControllerRawMethod method, const DataQuery &query)
{
    executeDataRequest(method, query, [this](const DataResponse &response) {
        if (!response.text().isEmpty() || !response.informativeText().isEmpty())
            showResponseMessage(QString(), response);
    });
}

void DataInterface::executeDataRequest(DataControllerRawMethod method, const DataQuery &query, const DataQueryResponseCallback &callback)
{
    AbstractDataController *controller = dataController();
    if (!controller)
        return;

    beginRequest();

    auto onProgress = [this](int processed, int total) {
        monitorRequest(processed, total);
    };

    auto onResponse = [this, callback](const DataResponse &response) {
        endRequest();
        callback(response);
    };

    (controller->*method)(prepareQuery(query), onProgress, onResponse);
}

QStringList DataInterface::s_availableOperations = { "search", "filter", "refresh", "showItem", "addItem", "editItem", "deleteItem" };

DataInterfacePrivate::DataInterfacePrivate(DataInterface *q, const QByteArray &id)
    : UserInterfacePrivate(q, id)
    , forge(new DataInterfaceForge(this))
    , dataController(nullptr)
{
}

DataInterfacePrivate::~DataInterfacePrivate()
{
    delete forge;
}

} // namespace Widgetry
