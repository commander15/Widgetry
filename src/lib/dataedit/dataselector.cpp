#include "dataselector.h"
#include "dataselector_p.h"
#include "ui_dataselector.h"

#include <Widgetry/databrowser.h>
#include <Widgetry/application.h>

#include <QtGui/qshortcut.h>

#include <QtWidgets/qscrollbar.h>

#include <DataGate/abstractdatamanager.h>
#include <DataGate/dataresponse.h>

using namespace DataGate;

namespace Widgetry {

DataSelector::DataSelector(DataBrowser *browser, Qt::WindowFlags flags)
    : DataSelector(static_cast<QWidget *>(browser), flags)
{
    d_ptr->manager = browser->dataManager();
}

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
    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &DataSelector::processDoubleClick);

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

    d_ptr->indicator = ui->loadIndicator;

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
    setModelObjects(Jsoner::Array());
}

void DataSelector::setLabel(const QString &field, const QString &name)
{
    int index = d_ptr->model.fields().indexOf(field);
    if (index >= 0 && index < d_ptr->model.columnCount())
        d_ptr->model.setHeaderData(index, Qt::Horizontal, name);
}

void DataSelector::setResizeMode(const QString &field, QHeaderView::ResizeMode mode)
{
    int index = d_ptr->model.fields().indexOf(field);
    if (index >= 0 && index < d_ptr->model.columnCount())
        ui->tableView->horizontalHeader()->setSectionResizeMode(index, mode);
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

Jsoner::Array DataSelector::modelObjects() const
{
    return d_ptr->model.array();
}

void DataSelector::setModelObjects(const Jsoner::Array &objects)
{
    d_ptr->model.setArray(objects);
    ui->stackedWidget->setCurrentIndex(objects.isEmpty() ? 0 : 1);
}

bool DataSelector::hasOption(SelectorOption option) const
{
    switch (option)
    {
        case SearchByText:
            return ui->searchInput->isVisible();

        case AlternatingRowColors:
            return ui->tableView->alternatingRowColors();

        case SingleSelection:
            return ui->tableView->selectionMode() == QAbstractItemView::SingleSelection;

        case MultiSelection:
            return ui->tableView->selectionMode() == QAbstractItemView::MultiSelection;

        case ExtendedSelection:
            return ui->tableView->selectionMode() == QAbstractItemView::ExtendedSelection;

        case ContiguousSelection:
            return ui->tableView->selectionMode() == QAbstractItemView::ContiguousSelection;

        case InfiniteLoad:
            return d_ptr->infiniteLoad;

        case AcceptOnDoubleClick:
            return d_ptr->acceptOnClick;

        case WithButtons:
            return ui->buttonBox->isVisible();
    }

    return false;
}

void DataSelector::setOption(SelectorOption option, bool on)
{
    switch (option) {
    case SearchByText:
        ui->searchInput->setVisible(on);
        break;

    case AlternatingRowColors:
        ui->tableView->setAlternatingRowColors(on);
        break;

    case SingleSelection:
        if (on)
            ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        else
            ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        break;

    case MultiSelection:
        if (on)
            ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
        else
            ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        break;

    case ExtendedSelection:
        if (on)
            ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        else
            ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
        break;

    case ContiguousSelection:
        if (on)
            ui->tableView->setSelectionMode(QAbstractItemView::ContiguousSelection);
        else
            ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
        break;

    case InfiniteLoad:
        d_ptr->infiniteLoad = true;
        break;

    case AcceptOnDoubleClick:
        if (d_ptr->acceptOnClick != on) {
            if (d_ptr->acceptOnClick)
                disconnect(ui->tableView, &QAbstractItemView::doubleClicked, this, &DataSelector::processDoubleClick);
            else
                connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &DataSelector::processDoubleClick);
            d_ptr->acceptOnClick = on;
        }
        break;

    case WithButtons:
        ui->buttonBox->setHidden(!on);
        break;
    }
}

QStringList DataSelector::fields() const
{
    return d_ptr->model.fields();
}

void DataSelector::setFields(const QStringList &fields)
{
    d_ptr->model.setFields(fields);

    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setVisible(fields.count() > 1);

    if (fields.count() == 1)
        header->setSectionResizeMode(0, QHeaderView::Stretch);
}

DataRequest DataSelector::request() const
{
    return d_ptr->request;
}

void DataSelector::setRequest(const DataGate::DataRequest &request)
{
    ui->searchInput->setText(request.query());
    d_ptr->request = request;

    if (!request.query().isEmpty())
        d_ptr->fetchData(request.query(), false);
}

DataGate::AbstractDataManager *DataSelector::manager() const
{
    return d_ptr->manager;
}

void DataSelector::setManager(DataGate::AbstractDataManager *manager)
{
    d_ptr->manager = manager;
}

void DataSelector::showEvent(QShowEvent *event)
{
    if (d_ptr->model.rowCount() == 0)
        d_ptr->fetchData(ui->searchInput->text(), false);
    QDialog::showEvent(event);
}

void DataSelector::fetchCompletions(const QString &query)
{
    if (!d_ptr->manager)
        return;

    DataRequest request;
    request.setQuery(query);

    d_ptr->manager->fetchSearchSuggestions(request, [this](const DataResponse &response) {
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
    d_ptr->request.setSort(d_ptr->model.fields().at(index), order);
    refresh();
}

void DataSelector::fetchData(const QString &query, bool force)
{
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
    d_ptr->fetchData(d_ptr->request.query(), true);
}

void DataSelector::processDoubleClick(const QModelIndex &index)
{
    if (index.isValid())
        accept();
}

void DataSelector::processResponse(const DataGate::DataResponse &response)
{
    if (response.isSuccess())
        setModelObjects(response.array());
}

DataSelectorPrivate::DataSelectorPrivate(DataSelector *q)
    : q_ptr(q)
    , indicator(nullptr)
    , infiniteLoad(true)
    , acceptOnClick(true)
    , manager(Application::instance()->dataManager())
{
}

void DataSelectorPrivate::fetchData(const QString &query, bool append)
{
    if (!manager || (append && !infiniteLoad))
        return;

    if (append && request.page() >= response.pageCount())
        return;

    indicator->show();

    request.setQuery(query);
    if (append)
        request.setPage(request.page() + 1);

    manager->fetchObjects(request, [append, this](const DataResponse &response) {
        if (append && response.page() == request.page()) {
            DataResponse newResponse(response);
            const Jsoner::Array source = response.array();
            Jsoner::Array target = model.array();
            std::copy(source.begin(), source.end(), std::back_inserter(target));
            newResponse.setArray(target);
            q_ptr->processResponse(newResponse);
        } else {
            q_ptr->processResponse(response);
        }

        if (response.isSuccess())
            this->response = response;

        indicator->hide();
    });
}

} // namespace Widgetry
