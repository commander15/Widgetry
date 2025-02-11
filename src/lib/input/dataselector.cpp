#include "dataselector.h"
#include "dataselector_p.h"
#include "ui_dataselector.h"

#include <Widgetry/abstractdatacontroller.h>
#include <Widgetry/dataresponse.h>

namespace Widgetry {

DataSelector::DataSelector(QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
    , ui(new Ui::DataSelector)
    , d_ptr(new DataSelectorPrivate())
{
    ui->setupUi(this);

    ui->tableView->setModel(&d_ptr->model);

    //connect(ui->searchInput, &QLineEdit::textChanged, this, &DataSelector::fetchCompletions);
    connect(ui->searchInput, &QLineEdit::textChanged, this, &DataSelector::autoSearch);
    connect(ui->refreshButton, &QAbstractButton::clicked, this, &DataSelector::refresh);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &DataSelector::closeDialog);

    connect(&d_ptr->model, &DataTableModel::fetchRequested, this, &DataSelector::refresh);
}

DataSelector::~DataSelector()
{
    delete ui;
}

QString DataSelector::searchText() const
{
    return ui->searchInput->text();
}

void DataSelector::setHeader(int index, const QString &name)
{
    d_ptr->model.setHeaderData(index, Qt::Horizontal, name);
}

void DataSelector::setResizeMode(int index, QHeaderView::ResizeMode mode)
{
    ui->tableView->horizontalHeader()->setSectionResizeMode(index, mode);
}

QStringList DataSelector::fields() const
{
    return d_ptr->model.fields();
}

void DataSelector::setFields(const QStringList &fields)
{
    d_ptr->model.setFields(fields);
}

Jsoner::Object DataSelector::selectedObject() const
{
    const QModelIndex index = ui->tableView->currentIndex();
    return (index.isValid() ? d_ptr->model.object(index.row()) : Jsoner::Object());
}

Jsoner::Array DataSelector::selectedObjects() const
{
    Jsoner::Array objects;
    const QModelIndexList indexes = ui->tableView->selectionModel()->selectedRows();
    const Jsoner::ItemModel *model = &d_ptr->model;

    for (const QModelIndex &index : indexes)
        objects.append(model->object(index.row()));

    return objects;
}

QTableView *DataSelector::tableView() const
{
    return ui->tableView;
}

AbstractDataController *DataSelector::dataController() const
{
    return d_ptr->dataController;
}

void DataSelector::setDataController(AbstractDataController *controller)
{
    d_ptr->dataController = controller;
}

void DataSelector::search(const QString &query)
{
    DataQuery dataQuery;
    dataQuery.setQuery(query);
    search(query);
}

void DataSelector::search(const DataQuery &query)
{
    ui->searchInput->setText(query.query());
    d_ptr->dataQuery = query;
    refresh();
}

void DataSelector::refresh()
{
    if (!d_ptr->dataController)
        return;

    d_ptr->dataQuery.setQuery(ui->searchInput->text());

    d_ptr->dataController->fetchObjects(d_ptr->dataQuery, [this](const DataResponse &response) {
        if (response.isSuccess())
            d_ptr->model.setArray(response.objects());
        else
            d_ptr->model.setArray(Jsoner::Array());
    });
}

void DataSelector::fetchCompletions(const QString &query)
{
    if (!d_ptr->dataController)
        return;

    DataQuery dataQuery;
    dataQuery.setQuery(query);

    d_ptr->dataController->fetchSuggestions(dataQuery, [this](const DataResponse &response) {
        if (!response.isSuccess())
            return;
    });
}

void DataSelector::autoSearch(const QString &query)
{
    if (d_ptr->dataSearchThreshold < 0 || query.size() < d_ptr->dataSearchThreshold)
        return;

    refresh();
}

void DataSelector::closeDialog()
{
    if (ui->tableView->currentIndex().isValid())
        accept();
    else
        reject();
}

} // namespace Widgetry
