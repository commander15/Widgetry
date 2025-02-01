#include "datainterface.h"
#include "datainterface_p.h"
#include "ui_datainterface.h"

#include <Widgetry/operation.h>
#include <Widgetry/abstractdatacontroller.h>
#include <Widgetry/dataedit.h>

#include <Jsoner/object.h>
#include <Jsoner/tablemodel.h>

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
    connect(ui->addButton, &QAbstractButton::clicked, this, &DataInterface::addItem);
    connect(ui->editButton, &QAbstractButton::clicked, this, &DataInterface::editCurrentItem);
    connect(ui->deleteButton, &QAbstractButton::clicked, this, &DataInterface::deleteSelectedItems);

    WIDGETRY_D(DataInterface);
    d->forge->init();
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

    AbstractDataController *oldController = d->dataController;
    AbstractDataController *newController = controller;

    if (oldController) {
        disconnect(oldController, &AbstractDataController::objectsFetched, this, &DataInterface::handleFetchedObjects);
        disconnect(oldController, &AbstractDataController::objectFetched, this, &DataInterface::handleFetchedObject);
        disconnect(oldController, &AbstractDataController::objectAdded, this, &DataInterface::handleAddedObject);
        disconnect(oldController, &AbstractDataController::objectEdited, this, &DataInterface::handleEditedObject);
        disconnect(oldController, &AbstractDataController::objectsDeleted, this, &DataInterface::handleDeletedObjects);
        disconnect(oldController, &AbstractDataController::errorOccured, this, &DataInterface::handleError);
    }

    if (newController) {
        connect(newController, &AbstractDataController::objectsFetched, this, &DataInterface::handleFetchedObjects);
        connect(newController, &AbstractDataController::objectFetched, this, &DataInterface::handleFetchedObject);
        connect(newController, &AbstractDataController::objectAdded, this, &DataInterface::handleAddedObject);
        connect(newController, &AbstractDataController::objectEdited, this, &DataInterface::handleEditedObject);
        connect(newController, &AbstractDataController::objectsDeleted, this, &DataInterface::handleDeletedObjects);
        connect(newController, &AbstractDataController::errorOccured, this, &DataInterface::handleError);
    }

    d->dataController = controller;
}

void DataInterface::search(const QString &query)
{
    ui->searchInput->setText(query);
    refresh();
}

void DataInterface::filter(const QVariantHash &filters)
{
    // ToDo: handle filters
    refresh();
}

void DataInterface::refresh()
{
    WIDGETRY_D(DataInterface);

    if (!d->dataController)
        return;

    d->dataController->fetchObjects(generateQuery());
}

void DataInterface::showCurrentItem()
{
    WIDGETRY_D(DataInterface);

    if (!d->dataController)
        return;

    d->dataController->fetchObject(currentObject(), AbstractDataController::FetchRequest);
}

void DataInterface::addItem()
{
    WIDGETRY_D(DataInterface);

    if (!d->dataController)
        return;

    Jsoner::Object object = addObject(Jsoner::Object());
    if (!object.isEmpty())
        d->dataController->addObject(object);
}

void DataInterface::editCurrentItem()
{
    WIDGETRY_D(DataInterface);

    if (!d->dataController)
        return;

    Jsoner::Object object = currentObject();
    if (!object.isEmpty())
        d->dataController->fetchObject(object, AbstractDataController::EditRequest);
}

void DataInterface::deleteSelectedItems()
{
    WIDGETRY_D(DataInterface);

    if (!d->dataController)
        return;

    const Jsoner::Array objects = selectedObjects();
    if (objects.empty())
        return;

    int response = QMessageBox::question(nullptr, tr("Deletion..."), tr("Do you really want to delete these %n record(s)", "", objects.size()));
    if (response == QMessageBox::No)
        return;

    d->dataController->deleteObjects(objects);
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

    if (!d->dataEdit)
        return;

    if (d->dataEditDialog) {
        d->dataEditDialog->exec();
        return;
    }
}

Jsoner::Object DataInterface::addObject(const Jsoner::Object &object)
{
    WIDGETRY_D(DataInterface);

    if (!d->dataEdit)
        return Jsoner::Object();

    d->dataEdit->add(object);

    if (d->dataEditDialog && d->dataEditDialog->exec())
        return d->dataEdit->object();

    return (d->dataEdit->isComplete() ? d->dataEdit->object() : Jsoner::Object());
}

Jsoner::Object DataInterface::editObject(const Jsoner::Object &object)
{
    WIDGETRY_D(DataInterface);

    if (!d->dataEdit)
        return Jsoner::Object();

    d->dataEdit->edit(object);

    if (d->dataEditDialog && d->dataEditDialog->exec())
        return d->dataEdit->object();

    return (d->dataEdit->isComplete() ? d->dataEdit->object() : Jsoner::Object());
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
    WIDGETRY_D(DataInterface);
    const QString query = parameters.value("query").toString();

    search(query);
    return true;
}

bool DataInterface::handleFilter(const QVariantHash &parameters)
{
    WIDGETRY_D(DataInterface);
    const QVariantHash filters = parameters.value("filters").toHash();

    filter(filters);
    return true;
}

bool DataInterface::handleRefresh(const QVariantHash &parameters)
{
    WIDGETRY_D(DataInterface);
    refresh();
    return true;
}

bool DataInterface::handleShowItem(const QVariantHash &parameters)
{
    WIDGETRY_D(DataInterface);
    const Jsoner::Object item = parameters.value("item").value<Jsoner::Object>();

    if (!d->dataController)
        return false;

    d->dataController->fetchObject(item, AbstractDataController::FetchRequest);
    return true;
}

bool DataInterface::handleAddItem(const QVariantHash &parameters)
{
    WIDGETRY_D(DataInterface);
    Jsoner::Object object = parameters.value("item").value<Jsoner::Object>();

    if (object.isEmpty())
        object = Jsoner::Object();

    if (!d->dataController)
        return false;

    d->dataController->addObject(object);
    return true;
}

bool DataInterface::handleEditItem(const QVariantHash &parameters)
{
    WIDGETRY_D(DataInterface);
    const Jsoner::Object object = parameters.value("item").value<Jsoner::Object>();

    if (!d->dataController)
        return false;

    d->dataController->editObject(object);
    return true;
}

bool DataInterface::handleDeleteItems(const QVariantHash &parameters)
{
    WIDGETRY_D(DataInterface);
    const Jsoner::Array objects = parameters.value("items").value<Jsoner::Array>();

    if (objects.isEmpty() || !d->dataController)
        return false;

    d->dataController->deleteObjects(objects);
    return true;
}

void DataInterface::handleOperationResult(const Operation &operation)
{
    Q_UNUSED(operation);
}

DataQuery DataInterface::generateQuery() const
{
    DataQuery query;
    query.setQuery(ui->searchInput->text());
    query.setPage(ui->pageInput->value());
    // ToDo: handle filters
    return query;
}

Jsoner::TableModel *DataInterface::createModel(const QStringList &fields)
{
    WIDGETRY_D(DataInterface);
    Jsoner::TableModel *model = new Jsoner::TableModel(this);
    d->forge->setTableModel(model);
    return model;
}

QMenu *DataInterface::createContextMenu(bool addDefaultActions)
{
    WIDGETRY_D(DataInterface);
    QMenu *menu = new QMenu(this);
    d->forge->setContextMenu(menu, addDefaultActions);
    return menu;
}

void DataInterface::handleFetchedObjects(const Jsoner::Array &objects, const DataResponse &response)
{
    WIDGETRY_D(DataInterface);

    ui->pageInput->setMaximum(response.pageCount());
    ui->pageInput->setValue(response.page());

    ui->previousPageButton->setEnabled(response.page() > 1);
    ui->nextPageButton->setEnabled(response.page() < response.pageCount());

    d->tableModel->setArray(objects);
}

void DataInterface::handleFetchedObject(const Jsoner::Object &object, const DataResponse &response, int targetRequestType)
{
    WIDGETRY_D(DataInterface);

    Jsoner::Object newObject;
    switch (targetRequestType) {
    case AbstractDataController::FetchRequest:
        showObject(object);
        break;

    case AbstractDataController::EditRequest:
        newObject = editObject(object);
        if (!newObject.isEmpty())
            d->dataController->addObject(newObject);
        break;

    default:
        break;
    }
    Q_UNUSED(response);
}

void DataInterface::handleAddedObject(const Jsoner::Object &object, const DataResponse &response)
{
    refresh();
    Q_UNUSED(object);
    Q_UNUSED(response);
}

void DataInterface::handleEditedObject(const Jsoner::Object &object, const DataResponse &response)
{
    refresh();
    Q_UNUSED(object);
    Q_UNUSED(response);
}

void DataInterface::handleDeletedObjects(const Jsoner::Array &objects, const DataResponse &response)
{
    refresh();
    Q_UNUSED(objects);
    Q_UNUSED(response);
}

void DataInterface::handleError(int requestType, const Jsoner::Array &objects, const DataResponse &error)
{
    Q_UNUSED(objects);

    QMessageBox box;
    box.setWindowTitle(tr("Error"));

    QString text = error.text();
    if (text.isEmpty()) {
        switch (requestType) {
        case AbstractDataController::FetchRequest:
            text = tr("Unable to retrieve data");
            break;

        case AbstractDataController::AddRequest:
            text = tr("Unable to add data");
            break;

        case AbstractDataController::EditRequest:
            text = tr("Unable to edit data");
            break;

        case AbstractDataController::DeleteRequest:
            text = tr("Unable to delete data");
            break;

        default:
            text = tr("Unknown error occurred");
            break;
        }
    }

    QString informativeText = error.informativeText();
    if (informativeText.isEmpty()) {
        informativeText = tr("Unknown error occured.");
    }

    box.setText("<b>" + text + "</b>");
    box.setInformativeText(informativeText);
    box.setDetailedText(error.detailedText());
    box.exec();
}

QStringList DataInterface::s_availableOperations = { "search", "filter", "refresh", "showItem", "addItem", "deleteItem" };

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
