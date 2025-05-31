#include "databrowser.h"
#include "databrowser_p.h"
#include "ui_databrowser.h"

#include <Widgetry/private/debug_p.h>
#include <Widgetry/abstractdataeditfactory.h>
#include <Widgetry/abstractdataedit.h>

#include <DataGate/dataquery.h>
#include <DataGate/dataresponse.h>

#include <QtGui/qevent.h>

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
    ui->tableView->installEventFilter(this);

    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    connect(selectionModel, &QItemSelectionModel::selectionChanged, d, &DataBrowserPrivate::adaptToSelection);

    connect(&d->tableModel, &TableModel::finished, this, &DataBrowser::processDataResponse);

    d->contextMenu = new QMenu(ui->tableView);
    d->showAction = d->contextMenu->addAction(QIcon(":/widgetry/icons/action_show.png"), QString());
    d->editAction = d->contextMenu->addAction(QIcon(":/widgetry/icons/action_edit.png"), QString());
    d->deleteAction = d->contextMenu->addAction(QIcon(":/widgetry/icons/action_delete.png"), QString());
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

    if (!d->tableModel.controller()) {
        widgetryWarning() << "DataBrowser::refresh(): no data controller set !";
        return;
    }

    d->tableModel.get(newQuery(RefreshQuery));
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
        d->tableModel.controller()->addObject(query, d->progressCallback, [this](const DataResponse &response) {
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
        d->tableModel.controller()->editObject(query, d->progressCallback, [this](const DataResponse &response) {
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

    if (!d->tableModel.controller()) {
        return;
    }

    d->tableModel.controller()->deleteObjects(items, [this](const DataResponse &response) {
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
    return d->tableModel.controller();
}

void DataBrowser::setDataController(DataGate::AbstractDataController *controller)
{
    WIDGETRY_D(DataBrowser);
    d->tableModel.setController(controller);
}

void DataBrowser::prepareUi()
{
    WIDGETRY_D(DataBrowser);
    if (d->tableModel.rowCount() == 0)
        refresh();
}

void DataBrowser::translateUi(bool full)
{
    WIDGETRY_D(DataBrowser);

    if (full)
        ui->retranslateUi(this);

    d->showAction->setText(tr("Show"));
    d->editAction->setText(tr("Edit"));
    d->deleteAction->setText(tr("Delete"));

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

bool DataBrowser::eventFilter(QObject *object, QEvent *event)
{
    WIDGETRY_D(DataBrowser);

    if (object == ui->tableView && event->type() == QEvent::ContextMenu) {
        d->contextMenu->popup(static_cast<QContextMenuEvent *>(event)->globalPos());
        return true;
    }

    return Widget::eventFilter(object, event);
}

DataBrowserPrivate::DataBrowserPrivate(DataBrowser *q, const QByteArray &id)
    : WidgetPrivate(q, id)
    , ui(nullptr)
    , contextMenu(nullptr)
    , editFactory(nullptr)
    , filterWidget(nullptr)
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

    AbstractDataController *controller = tableModel.controller();

    if (!controller) {
        widgetryWarning() << q->title() << ": no data controller set !";
        return;
    }

    controller->fetchObject(q->newQuery(DataBrowser::ShowQuery), nullptr, [=](const DataResponse &response) {
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
    AbstractDataController *dataController = tableModel.controller();
    return dataController ? dataController->hasFeature(feature, q) : false;
}

void DataBrowserPrivate::adaptToSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
    const bool single = selected.count() == 1;
    const bool multiple = selected.count() > 1;

    ui->editButton->setVisible(single && hasDataFeature(AbstractDataController::UpdateObject));
    ui->deleteButton->setVisible((single || multiple) && hasDataFeature(AbstractDataController::DeleteObjects));

    showAction->setVisible(single);
    editAction->setVisible(single && hasDataFeature(AbstractDataController::UpdateObject));
    deleteAction->setVisible((single || multiple) && hasDataFeature(AbstractDataController::DeleteObjects));
}

} // namespace Widgetry
