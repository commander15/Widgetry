#include "dataselectorwidget.h"
#include "dataselectorwidget_p.h"
#include "ui_dataselectorwidget.h"

#include <Widgetry/abstractdatacontroller.h>
#include <Widgetry/dataresponse.h>

namespace Widgetry {

DataSelectorWidget::DataSelectorWidget(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
    , ui(new Ui::DataSelectorWidget)
    , d_ptr(new DataSelectorWidgetPrivate)
{
    ui->setupUi(this);

    ui->tableView->setModel(&d_ptr->model);

    //connect(ui->searchInput, &QLineEdit::textChanged, this, &DataSelector::fetchCompletions);
    connect(ui->searchInput, &QLineEdit::textChanged, this, &DataSelectorWidget::autoSearch);
    connect(ui->refreshButton, &QAbstractButton::clicked, this, &DataSelectorWidget::refresh);

    connect(&d_ptr->model, &DataTableModel::fetchRequested, this, &DataSelectorWidget::refresh);

    addAction(ui->actionGoToSearchBar);
    addAction(ui->actionTriggerSearch);
}

DataSelectorWidget::~DataSelectorWidget()
{
    delete ui;
}

QString DataSelectorWidget::searchText() const
{
    return ui->searchInput->text();
}

DataQuery DataSelectorWidget::searchQuery() const
{
    return d_ptr->dataQuery;
}

void DataSelectorWidget::setSearchQuery(const DataQuery &query)
{
    ui->searchInput->setText(query.query());
    d_ptr->dataQuery = query;
}

void DataSelectorWidget::setHeader(int index, const QString &name)
{
    d_ptr->model.setHeaderData(index, Qt::Horizontal, name);
}

void DataSelectorWidget::setResizeMode(int index, QHeaderView::ResizeMode mode)
{
    ui->tableView->horizontalHeader()->setSectionResizeMode(index, mode);
}

QStringList DataSelectorWidget::fields() const
{
    return d_ptr->model.fields();
}

void DataSelectorWidget::setFields(const QStringList &fields)
{
    d_ptr->model.setFields(fields);
}

Jsoner::Object DataSelectorWidget::selectedObject() const
{
    const QModelIndex index = ui->tableView->currentIndex();
    return (index.isValid() ? d_ptr->model.object(index.row()) : Jsoner::Object());
}

Jsoner::Array DataSelectorWidget::selectedObjects() const
{
    Jsoner::Array objects;
    const QModelIndexList indexes = ui->tableView->selectionModel()->selectedRows();
    const Jsoner::ItemModel *model = &d_ptr->model;

    for (const QModelIndex &index : indexes)
        objects.append(model->object(index.row()));

    return objects;
}

QTableView *DataSelectorWidget::tableView() const
{
    return ui->tableView;
}

AbstractDataController *DataSelectorWidget::dataController() const
{
    return d_ptr->dataController;
}

void DataSelectorWidget::setDataController(AbstractDataController *controller)
{
    d_ptr->dataController = controller;
}

void DataSelectorWidget::search(const QString &query)
{
    ui->searchInput->setText(query);
    refresh();
}

void DataSelectorWidget::search(const DataQuery &query)
{
    ui->searchInput->setText(query.query());
    d_ptr->dataQuery = query;
    refresh();
}

void DataSelectorWidget::refresh()
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

void DataSelectorWidget::clear()
{
    ui->searchInput->clear();
    d_ptr->model.setArray(Jsoner::Array());
}

void DataSelectorWidget::fetchCompletions(const QString &query)
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

void DataSelectorWidget::autoSearch(const QString &query)
{
    if (d_ptr->dataSearchThreshold < 0 || query.size() < d_ptr->dataSearchThreshold)
        return;

    refresh();
}

} // namespace Widgetry
