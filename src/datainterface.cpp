#include "datainterface.h"
#include "datainterface_p.h"
#include "ui_datainterface.h"

#include <Widgetry/operation.h>

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
    : UserInterface(new DataInterfacePrivate(id, this), parent, flags)
    , DataInterfaceBase(new Ui::DataInterface, static_cast<DataInterfacePrivate *>(d_ptr.data()))
{
    ui->setupUi(this);
}

DataInterface::~DataInterface()
{
    delete ui;
}

bool DataInterface::isOperationSupported(const QString &operation) const
{
    static const QStringList operations = { "search", "filter", "refresh", "showItem", "addItem", "deleteItem" };
    return operations.contains(operation);
}

void DataInterface::search(const QString &query)
{
    ui->searchInput->setText(query);
    fetchObjects();
}

void DataInterface::filter(const QVariantHash &filters)
{
    // ToDo: implement filtering logic
    fetchObjects();
}

void DataInterface::refresh()
{
    fetchObjects();
}

void DataInterface::addItem()
{
    addObject(Jsoner::Object());
}

void DataInterface::editItem()
{
    editObject(currentObject());
}

void DataInterface::deleteItems()
{
    const QList<Jsoner::Object> objects = selectedObjects();
    if (canDeleteObjects(objects))
        deleteObjects(objects);
}

void DataInterface::sync()
{
    //
}

bool DataInterface::handleOperation(Operation *operation)
{
    const QString name = operation->name();
    const QVariantHash parameters = operation->parameters();

    if (name == "search") {
        search(parameters.value("query").toString());
    } else if (name == "filter") {
        filter(parameters.value("filters").toHash());
    } else if (name == "refresh") {
        refresh();
    } else if (name == "showItem") {
        showObject(parameters.value("item").value<Jsoner::Object>());
    } else if (name == "addItem") {
        if (parameters.contains("item"))
            addObject(parameters.value("item").value<Jsoner::Object>());
        else
            addObject(Jsoner::Object());
    } else if (name == "editItem") {
        editObject(parameters.value("item").value<Jsoner::Object>());
    } else if (name == "deleteItems") {
        const QList<Jsoner::Object> objects = parameters.value("items").value<QList<Jsoner::Object>>();
        if (canDeleteObjects(objects))
            deleteObjects(objects);
    } else {
        return false;
    }

    return true;
}

void DataInterface::handleOperationResult(const Operation &operation)
{
    Q_UNUSED(operation);
}

bool DataInterface::canDeleteObjects(const QList<Jsoner::Object> &objects)
{
    if (objects.isEmpty())
        return false;

    int response = QMessageBox::question(nullptr, tr("Deletion..."), tr("Do you really want to delete these %n record(s)", "", objects.size()));
    if (response == QMessageBox::No)
        return false;

    return true;
}

Jsoner::TableModel *DataInterface::createModel(const QStringList &fields)
{
    Jsoner::TableModel *model = new Jsoner::TableModel(this);
    setTableModel(model);
    return model;
}

QMenu *DataInterface::createContextMenu(bool addDefaultActions)
{
    QMenu *menu = new QMenu(this);
    setContextMenu(menu, addDefaultActions);
    return menu;
}

} // namespace Widgetry
