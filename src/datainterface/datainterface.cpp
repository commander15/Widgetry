#include "datainterface.h"
#include "datainterface_p.h"
#include "ui_datainterface.h"

#include <Widgetry/operation.h>
#include <Widgetry/abstractdatacontroller.h>
#include <Widgetry/dataquery.h>
#include <Widgetry/dataresponse.h>
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

    executeDataRequest(&AbstractDataController::fetchObjects, generateQuery(), [this, d](const DataResponse &response) {
        if (!response.isSuccess()) {
            showResponseMessage(tr("Error during data download !"), response);
            return;
        }

        d->tableModel->setArray(response.objects());

        ui->pageInput->setMaximum(response.pageCount());
        ui->pageInput->setValue(response.page());

        ui->previousPageButton->setEnabled(response.page() > 1);
        ui->nextPageButton->setEnabled(response.page() < response.pageCount());
    });
}

void DataInterface::showCurrentItem()
{
    WIDGETRY_D(DataInterface);

    if (!d->dataController)
        return;

    Jsoner::Object object = currentObject();
    if (!object.isEmpty()) {
        executeDataRequest(&AbstractDataController::fetchObject, object, [this](const DataResponse &response) {
            if (response.isSuccess())
                showObject(response.object());
            else
                showResponseMessage(tr("Error during data download !"), response);
        });
    }
}

void DataInterface::addItem()
{
    WIDGETRY_D(DataInterface);

    if (!d->dataController)
        return;

    auto process = [this](const DataResponse &response) {
        if (response.isSuccess()) {
            refresh();
            return;
        }

        showResponseMessage(tr("Error during data download !"), response);
        return;
        int answer = QMessageBox::question(nullptr, tr("Operation failed"), tr("Do you want to retry ?"));
    };

    Jsoner::Object object = addObject(Jsoner::Object());
    if (!object.isEmpty())
        executeDataRequest(&AbstractDataController::addObject, object, process);

}

void DataInterface::editCurrentItem()
{
    WIDGETRY_D(DataInterface);

    if (!d->dataController)
        return;

    Jsoner::Object object = currentObject();
    if (!object.isEmpty()) {
        executeDataRequest(&AbstractDataController::fetchObject, object, [this](const DataResponse &response) {
            if (!response.isSuccess()) {
                showResponseMessage(tr("Error during data download !"), response);
                return;
            }

            const Jsoner::Object object = editObject(response.object());
                if (!object.isEmpty()) {
                    executeDataRequest(&AbstractDataController::editObject, object, [this](const DataResponse &response) {
                        if (response.isSuccess()) {
                            refresh();
                            return;
                        }

                        showResponseMessage(tr("Error during data saving"), response);
                    });
                }
        });
    }
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

    executeDataRequest(&AbstractDataController::deleteObjects, objects, [this](const DataResponse &response) {
        if (response.isSuccess()) {
            refresh();
            return;
        }

        showResponseMessage(tr("Error during deletion !"), response);
    });
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
    const Jsoner::Object object = parameters.value("item").value<Jsoner::Object>();

    if (!d->dataController)
        return false;

    executeDataRequest(&AbstractDataController::fetchObject, object, [this](const DataResponse &response) {
        if (response.isSuccess())
            showObject(response.object());
    });
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

    executeDataRequest(&AbstractDataController::addObject, object);
    return true;
}

bool DataInterface::handleEditItem(const QVariantHash &parameters)
{
    WIDGETRY_D(DataInterface);
    const Jsoner::Object object = parameters.value("item").value<Jsoner::Object>();

    if (!d->dataController)
        return false;

    executeDataRequest(&AbstractDataController::editObject, object);
    return true;
}

bool DataInterface::handleDeleteItems(const QVariantHash &parameters)
{
    WIDGETRY_D(DataInterface);
    const Jsoner::Array objects = parameters.value("items").value<Jsoner::Array>();

    if (objects.isEmpty() || !d->dataController)
        return false;

    executeDataRequest(&AbstractDataController::deleteObjects, objects);
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

void DataInterface::showResponseMessage(const QString &title, const DataResponse &response)
{
    QMessageBox box;
    box.setWindowTitle(!title.isEmpty() ? title : (response.isSuccess() ? tr("Message") : tr("Error")));
    box.setIcon(response.isSuccess() ? QMessageBox::Information : QMessageBox::Warning);

    box.setText("<b>" + response.text() + "</b>");
    box.setInformativeText(response.informativeText());
    box.setDetailedText(response.detailedText());
    box.exec();
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

    (controller->*method)(query, onProgress, onResponse);
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
