#include "databrowser.h"
#include "databrowser_p.h"
#include "ui_databrowser.h"

#include <Widgetry/private/debug_p.h>
#include <Widgetry/abstractdataeditfactory.h>
#include <Widgetry/abstractdataedit.h>

#include <DataGate/dataquery.h>
#include <DataGate/dataresponse.h>

#include <QtWidgets/qmessagebox.h>

using namespace DataGate;

namespace Widgetry {

DataBrowser::DataBrowser(const QByteArray &id, QWidget *parent, Qt::WindowFlags flags)
    : Widget(new DataBrowserPrivate(this, id), parent, flags)
    , ui(new Ui::DataBrowser())
{
    WIDGETRY_D(DataBrowser);
    d->ui = ui;

    ui->setupUi(this);

    ui->tableView->setModel(&d->tableModel);

    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    connect(selectionModel, &QItemSelectionModel::selectionChanged, d, &DataBrowserPrivate::adaptToSelection);

    connect(&d->tableModel, &TableModel::finished, this, &DataBrowser::processDataResponse);
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
    refresh();
}

void DataBrowser::refresh()
{
    WIDGETRY_D(DataBrowser);

    if (!d->dataController) {
        widgetryWarning() << "DataBrowser::refresh(): no data controller set !";
        return;
    }

    DataQuery query = newQuery(RefreshQuery);
    d->tableModel.get(query);
}

Jsoner::Object DataBrowser::currentObject() const
{
    WIDGETRY_D(const DataBrowser);

    const QModelIndex index = ui->tableView->currentIndex();
    return (index.isValid() ? d->tableModel.object(index.row()) : Jsoner::Object());
}

Jsoner::Array DataBrowser::selectedObjects() const
{
    WIDGETRY_D(const DataBrowser);

    const QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();
    Jsoner::Array objects;

    std::transform(selectedIndexes.begin(), selectedIndexes.end(), std::back_inserter(objects), [d](const QModelIndex &index) {
        return d->tableModel.object(index.row());
    });

    return objects;
}

void DataBrowser::showCurrentItem()
{
    showItem(currentObject());
}

void DataBrowser::showItem(const Jsoner::Object &item)
{
    WIDGETRY_D(DataBrowser);
    d->openEdit(item, AbstractDataEdit::ShowOperation, nullptr);
}

void DataBrowser::addNewItem()
{
    addItem(Jsoner::Object());
}

void DataBrowser::addItem(const Jsoner::Object &item)
{
    WIDGETRY_D(DataBrowser);
    d->openEdit(item, AbstractDataEdit::AddOperation, [=](const Jsoner::Object &object) {
        DataQuery query = newQuery(AddQuery);
        query.setObject(object);
        d->dataController->addObject(query, d->progressCallback, [this](const DataResponse &response) {
            processDataResponse(response);
        });
    });
}

void DataBrowser::editCurrentItem()
{
    editItem(currentObject());
}

void DataBrowser::editItem(const Jsoner::Object &item)
{
    WIDGETRY_D(DataBrowser);
    d->openEdit(item, AbstractDataEdit::EditOperation, [=](const Jsoner::Object &object) {
        DataQuery query = newQuery(EditQuery);
        query.setObject(object);
        d->dataController->editObject(query, d->progressCallback, [this](const DataResponse &response) {
            processDataResponse(response);
        });
    });
}

void DataBrowser::deleteSelectedItems()
{
    deleteItems(selectedObjects());
}

void DataBrowser::deleteItems(const Jsoner::Array &items)
{
    WIDGETRY_D(DataBrowser);

    if (!d->dataController) {
        return;
    }

    d->dataController->deleteObjects(items, [this](const DataResponse &response) {
        processDataResponse(response);
    });
}

void DataBrowser::sync()
{
    WIDGETRY_D(DataBrowser);

    ui->searchInput->setVisible(d->hasDataFeature(AbstractDataController::SearchByKeywordsSearch));
    ui->filterButton->setVisible(d->hasDataFeature(AbstractDataController::SearchByFilters));
    ui->filterContainer->setVisible(ui->filterButton->isVisible());
    ui->tableView->setSortingEnabled(d->hasDataFeature(AbstractDataController::SortedResults));

    ui->addButton->setVisible(d->hasDataFeature(AbstractDataController::CreateObject));

    // Need ajustments to fit selection
    d->adaptToSelection(ui->tableView->selectionModel()->selection(), QItemSelection());

    Widget::sync();
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
}

AbstractDataController *DataBrowser::dataController() const
{
    WIDGETRY_D(const DataBrowser);
    return d->dataController;
}

void DataBrowser::setDataController(DataGate::AbstractDataController *controller)
{
    WIDGETRY_D(DataBrowser);
    d->dataController = controller;
}

void DataBrowser::prepareUi()
{
    WIDGETRY_D(DataBrowser);
    if (d->tableModel.rowCount() == 0)
        refresh();
}

void DataBrowser::translateUi(bool full)
{
    if (full)
        ui->retranslateUi(this);
    Widget::translateUi(full);
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
        query.setPage(ui->pageInput->value());
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

bool DataBrowser::processDataResponse(const DataResponse &response)
{
    WIDGETRY_D(DataBrowser);

    if (sender() == &d->tableModel && response.isSuccess()) {
        const Jsoner::Object object = response.object();
        ui->pageInput->setMaximum(response.pageCount());
        ui->pageInput->setValue(response.page());
        ui->previousPageButton->setVisible(response.page() > 1);
        ui->nextPageButton->setVisible(response.page() < response.pageCount());
    }

    if (response.hasMessage()) {
        QMessageBox box;
        box.setText(response.text());
        box.setInformativeText(response.informativeText());
        box.setDetailedText(response.detailedText());

        if (response.isSuccess()) {
            box.setWindowTitle(response.title().isEmpty() ? tr("Message") : response.title());
            box.setIcon(QMessageBox::Information);
        } else {
            box.setWindowTitle(response.title().isEmpty() ? tr("Error") : response.title());
            box.setIcon(QMessageBox::Warning);
        }

        box.exec();
    }

    return false;
}

DataBrowserPrivate::DataBrowserPrivate(DataBrowser *q, const QByteArray &id)
    : WidgetPrivate(q, id)
    , ui(nullptr)
    , editFactory(nullptr)
    , filterWidget(nullptr)
    , dataController(nullptr)
{
    progressCallback = [](qint64 current, qint64 total) {
        widgetryInfo() << "Request progress";
    };
}

void DataBrowserPrivate::openEdit(const QJsonObject &item, AbstractDataEdit::Operation operation, const EditingCallback &callback)
{
    WIDGETRY_Q(DataBrowser);

    if (!editFactory) {
        widgetryWarning() << q->title() << ": no edit factory set !";
        return;
    }

    if (!dataController) {
        widgetryWarning() << q->title() << ": no data controller set !";
        return;
    }

    dataController->fetchObject(q->newQuery(DataBrowser::ShowQuery), nullptr, [=](const DataResponse &response) {
        if (q->processDataResponse(response) || !response.isSuccess())
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

bool DataBrowserPrivate::hasDataFeature(DataGate::AbstractDataController::Feature feature) const
{
    WIDGETRY_Q(DataBrowser);
    return dataController ? dataController->hasFeature(feature, q) : false;
}

void DataBrowserPrivate::adaptToSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
    ui->editButton->setVisible(selected.count() == 1 && hasDataFeature(AbstractDataController::UpdateObject));
    ui->deleteButton->setVisible(!selected.isEmpty() && hasDataFeature(AbstractDataController::DeleteObjects));
}

} // namespace Widgetry
