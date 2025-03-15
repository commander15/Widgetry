#include "dataselector.h"
#include "ui_dataselector.h"

#include <DataGate/abstractdatacontroller.h>
#include <DataGate/dataquery.h>
#include <DataGate/dataresponse.h>

#include <Jsoner/object.h>
#include <Jsoner/array.h>

#include <QtWidgets/qtableview.h>

namespace Widgetry {

DataSelector::DataSelector(QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
    , ui(new Ui::DataSelector)
{
    ui->setupUi(this);

    ui->selector->layout()->setContentsMargins(QMargins());
    connect(ui->selector->tableView(), &QAbstractItemView::activated, this, [this](const QModelIndex &index) {
        if (index.isValid())
            accept();
    });

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &DataSelector::closeDialog);

    addActions(ui->selector->actions());
}

DataSelector::~DataSelector()
{
    delete ui;
}

QString DataSelector::searchText() const
{
    return ui->selector->searchText();
}

DataGate::DataQuery DataSelector::searchQuery() const
{
    return ui->selector->searchQuery();
}

void DataSelector::setSearchQuery(const DataGate::DataQuery &query)
{
    ui->selector->setSearchQuery(query);
}

void DataSelector::setHeader(int index, const QString &name)
{
    ui->selector->setHeader(index, name);
}

void DataSelector::setResizeMode(int index, QHeaderView::ResizeMode mode)
{
    ui->selector->setResizeMode(index, mode);
}

QStringList DataSelector::fields() const
{
    return ui->selector->fields();
}

void DataSelector::setFields(const QStringList &fields)
{
    ui->selector->setFields(fields);
}

Jsoner::Object DataSelector::selectedObject() const
{
    return ui->selector->selectedObject();
}

Jsoner::Array DataSelector::selectedObjects() const
{
    return ui->selector->selectedObjects();
}

QTableView *DataSelector::tableView() const
{
    return ui->selector->tableView();
}

DataGate::AbstractDataController *DataSelector::dataController() const
{
    return ui->selector->dataController();
}

void DataSelector::setDataController(DataGate::AbstractDataController *controller)
{
    ui->selector->setDataController(controller);
}

DataSelectorWidget *DataSelector::selectorWidget() const
{
    return ui->selector;
}

void DataSelector::search(const QString &query)
{
    ui->selector->search(query);
}

void DataSelector::search(const DataGate::DataQuery &query)
{
    ui->selector->search(query);
}

void DataSelector::refresh()
{
    ui->selector->refresh();
}

void DataSelector::clear()
{
    ui->selector->clear();
}

void DataSelector::closeDialog()
{
    if (ui->selector->tableView()->currentIndex().isValid())
        accept();
    else
        reject();
}

} // namespace Widgetry
