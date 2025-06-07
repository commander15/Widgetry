#include "dataselector.h"
#include "dataselector_p.h"
#include "ui_dataselector.h"

#include <QtGui/qshortcut.h>

#include <QtWidgets/qscrollbar.h>

#include <DataGate/abstractdatamanager.h>
#include <DataGate/dataresponse.h>

using namespace DataGate;

namespace Widgetry {

DataSelector::DataSelector(QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
    , ui(new Ui::DataSelector)
    , d_ptr(new DataSelectorPrivate(this))
{
    ui->setupUi(this);

    ui->searchInput->setCompletionMode(SearchBar::InlineCompletion);
    connect(ui->searchInput, &SearchBar::completionsRequested, this, &DataSelector::fetchCompletions);
    connect(ui->searchInput, &SearchBar::completionsRequested, this, [this](const QString &query) { fetchData(query, false); });
    connect(ui->searchInput, &SearchBar::searchRequested, this, &DataSelector::refresh);

    ui->tableView->setModel(&d_ptr->model);

    QHeaderView *view = ui->tableView->horizontalHeader();
    connect(view, &QHeaderView::sortIndicatorChanged, this, &DataSelector::sortData);

    QScrollBar *scroll = ui->tableView->verticalScrollBar();
    connect(scroll, &QScrollBar::valueChanged, this,[scroll, this](int value) {
        if (value == scroll->maximum())
            fetchMore();
    });

    QShortcut *refresh = new QShortcut(QKeySequence::Refresh, this, this, &DataSelector::refresh);
    Q_UNUSED(refresh);

    QShortcut *gotoSearchBar = new QShortcut(QKeySequence::Find, this, ui->searchInput, QOverload<>::of(&QWidget::setFocus));
    Q_UNUSED(gotoSearchBar);

    d_ptr->requestTimer.setInterval(500);
    d_ptr->requestTimer.setSingleShot(true);
    connect(&d_ptr->requestTimer, &QTimer::timeout, this, &DataSelector::refresh);
}

DataSelector::~DataSelector()
{
    delete ui;
}

QString DataSelector::searchText() const
{
    return ui->searchInput->text();
}

void DataSelector::search(const QString &query)
{
    ui->searchInput->setText(query);
    fetchData(query, true);
}

void DataSelector::refresh()
{
    const QString query = ui->searchInput->text();
    fetchData(ui->searchInput->hasSelectedText() ? query.mid(0, ui->searchInput->selectionStart()) : query, true);
}

void DataSelector::clear()
{
    ui->searchInput->clear();
    d_ptr->model.setArray(Jsoner::Array());
}

void DataSelector::setLabel(int index, const QString &name)
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

bool DataSelector::hasOption(SelectorOption option) const
{
    switch (option)
    {
        case SearchByText:
            return ui->searchInput->isVisible();

        case InfiniteLoad:
            return d_ptr->infiniteLoad;

        case NoButtons:
            return ui->buttonBox->isHidden();
    }

    return false;
}

void DataSelector::setOption(SelectorOption option, bool on)
{
    switch (option)
    {
    case SearchByText:
        ui->searchInput->setVisible(on);
        if (on)
            ui->stackedWidget->setCurrentIndex(0);
        break;

    case InfiniteLoad:
        d_ptr->infiniteLoad = true;
        break;

    case NoButtons:
        ui->buttonBox->setHidden(on);
        if (on)
            ui->stackedWidget->setCurrentIndex(1);
        break;
    }
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

DataQuery DataSelector::searchQuery() const
{
    return d_ptr->query;
}

void DataSelector::setSearchQuery(const DataGate::DataQuery &query)
{
    ui->searchInput->setText(query.string());
    d_ptr->query = query;
}

DataGate::AbstractDataManager *DataSelector::dataManager() const
{
    return d_ptr->manager;
}

void DataSelector::setDataManager(DataGate::AbstractDataManager *manager)
{
    d_ptr->manager = manager;
}

void DataSelector::fetchCompletions(const QString &query)
{
    if (!d_ptr->manager)
        return;

    DataQuery dataQuery;
    dataQuery.setString(query);

    d_ptr->manager->fetchSearchSuggestions(dataQuery, [this](const DataResponse &response) {
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

void DataSelector::sortData(int index, Qt::SortOrder order)
{
    d_ptr->query.setSort(d_ptr->model.fields().at(index), order);
    refresh();
}

void DataSelector::fetchData(const QString &query, bool force)
{
    if (query.isEmpty()) {
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }

    if (force) {
        if (d_ptr->requestTimer.isActive())
            d_ptr->requestTimer.stop();
        d_ptr->fetchData(query, false);
        return;
    }

    d_ptr->requestTimer.start();
}

void DataSelector::fetchMore()
{
    d_ptr->fetchData(d_ptr->query.string(), true);
}

void DataSelector::processResponse(const DataGate::DataResponse &response)
{
    if (!response.isSuccess())
        return;

    d_ptr->model.setArray(response.array());
    ui->stackedWidget->setCurrentIndex(response.array().isEmpty() ? 0 : 1);
}

DataSelectorPrivate::DataSelectorPrivate(DataSelector *q)
    : q_ptr(q)
    , infiniteLoad(true)
    , manager(nullptr)
{
}

void DataSelectorPrivate::fetchData(const QString &query, bool append)
{
    if (!manager || (append && !infiniteLoad))
        return;

    if (append && this->query.page() >= this->response.pageCount())
        return;

    this->query.setString(query);
    if (append)
        this->query.setPage(this->query.page() + 1);

    manager->fetchObjects(this->query, [append, this](const DataResponse &response) {
        if (append && response.page() == this->query.page()) {
            DataResponse newResponse(response);
            Jsoner::Array target = model.array();
            const Jsoner::Array source = response.array();
            std::copy(source.begin(), source.end(), std::back_inserter(target));
            newResponse.setArray(target);
            q_ptr->processResponse(newResponse);
        } else {
            q_ptr->processResponse(response);
        }

        if (response.isSuccess())
            this->response = response;
    });
}

} // namespace Widgetry
