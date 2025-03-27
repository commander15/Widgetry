#include "datainterfaceforge.h"
#include "datainterfaceforge_p.h"

#include "ui_datainterface.h"

#include <Widgetry/datainterface.h>
#include <Widgetry/dataedit.h>

#include <DataGate/tablemodel.h>
#include <DataGate/dataquery.h>
#include <DataGate/dataresponse.h>
#include <DataGate/permissionmanager.h>

#include <QtWidgets/qmenu.h>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qscrollbar.h>
#include <QtWidgets/qabstractitemview.h>
#include <QtWidgets/qcompleter.h>
#include <QtWidgets/qmessagebox.h>

#include <QtCore/qitemselectionmodel.h>
#include <QtCore/qstringlistmodel.h>

namespace Widgetry {

DataInterfaceForge::DataInterfaceForge(DataInterfaceForgePrivate *d)
    : ui(nullptr)
    , d_ptr(d)
{
}

DataInterfaceForge::~DataInterfaceForge()
{
}

QWidget *DataInterfaceForge::topBar() const
{
    return ui->topBar;
}

QLineEdit *DataInterfaceForge::searchBar() const
{
    return ui->searchInput;
}

QAbstractButton *DataInterfaceForge::filterToggleButton() const
{
    return ui->toggleFiltersButtons;
}

QAbstractButton *DataInterfaceForge::refreshButton() const
{
    return ui->refreshButton;
}

QAbstractButton *DataInterfaceForge::addButton() const
{
    return ui->addButton;
}

QAbstractButton *DataInterfaceForge::editButton() const
{
    return ui->editButton;
}

QAbstractButton *DataInterfaceForge::deleteButton() const
{
    return ui->deleteButton;
}

void DataInterfaceForge::addButton(QAbstractButton *button)
{
    ui->topBarLayout->addWidget(button);
}

void DataInterfaceForge::insertButton(QAbstractButton *before, QAbstractButton *button)
{
    int index = ui->topBarLayout->indexOf(before);
    ui->topBarLayout->insertWidget(index, button);
}

void DataInterfaceForge::removeButton(QAbstractButton *button)
{
    ui->topBarLayout->removeWidget(button);
}

AbstractDataEdit *DataInterfaceForge::filterWidget() const
{
    return d_ptr->filterWidget;
}

QAbstractButton *DataInterfaceForge::filterButton() const
{
    return ui->filterButton;
}

QTableView *DataInterfaceForge::tableView() const
{
    return ui->tableView;
}

DataGate::TableModel *DataInterfaceForge::tableModel() const
{
    return static_cast<DataGate::TableModel *>(ui->tableView->model());
}

QSpinBox *DataInterfaceForge::pageInput() const
{
    return ui->pageInput;
}

QAbstractButton *DataInterfaceForge::previousPageButton() const
{
    return ui->previousPageButton;
}

QAbstractButton *DataInterfaceForge::nextPageButton() const
{
    return ui->nextPageButton;
}

QMenu *DataInterfaceForge::contextMenu() const
{
    return d_ptr->contextMenu;
}

AbstractDataEditFactory *DataInterfaceForge::dataEditFactory() const
{
    return d_ptr->dataEditFactory;
}

bool DataInterfaceForge::isBluePrinted() const
{
    return d_ptr->bluePrinted;
}

void DataInterfaceForge::setFilterWidget(AbstractDataEdit *widget)
{
    if (!widget || d_ptr->filterWidget)
        return;

    ui->filterContainerLayout->addWidget(widget->editWidget());
    d_ptr->filterWidget = widget;

    emit operationSupportChanged("filter");
}

void DataInterfaceForge::setDataEdit(AbstractDataEditFactory *factory)
{
    d_ptr->dataEditFactory = factory;

    const bool enable = factory;
    emit operationSupportChanged("showItem");
    emit operationSupportChanged("addItem");
    emit operationSupportChanged("editItem");
}

void DataInterfaceForge::setContextMenu(QMenu *menu, bool addDefaultActions)
{
    if (addDefaultActions) {
        d_ptr->showAction = new QAction(QIcon(":/widgetry/icons/action_show.png"), QString(), menu);
        connect(d_ptr->showAction, &QAction::triggered, d_ptr->forgeInterface(), &DataInterface::showCurrentItem);

        d_ptr->editAction = new QAction(ui->editButton->icon(), QString(), menu);
        connect(d_ptr->editAction, &QAction::triggered, d_ptr->forgeInterface(), &DataInterface::editCurrentItem);

        d_ptr->deleteAction = new QAction(ui->deleteButton->icon(), QString(), menu);
        connect(d_ptr->deleteAction, &QAction::triggered, d_ptr->forgeInterface(), &DataInterface::deleteSelectedItems);

        QList<QAction *> actions = menu->actions();
        QAction *first = (!actions.isEmpty() ? actions.first() : nullptr);

        actions = { d_ptr->showAction, d_ptr->editAction, d_ptr->deleteAction };
        if (first) {
            QAction *separator = new QAction(menu);
            separator->setSeparator(true);
            actions.append(separator);
        }

        menu->insertActions(first, actions);
    }

    connect(ui->tableView, &QWidget::customContextMenuRequested, menu, [this, menu](const QPoint &pos) {
        DataInterface *interface = d_ptr->forgeInterface();;
        interface->showContextMenu(interface->selectedObjects(), pos);
    });

    d_ptr->contextMenu = menu;
}

void DataInterfaceForge::setDataController(DataGate::AbstractDataController *controller)
{
    d_ptr->tableModel->setController(controller);
    d_ptr->dataController = controller;

    emit operationSupportChanged("refresh");
    emit operationSupportChanged("search");
    emit operationSupportChanged("filter");
    emit operationSupportChanged("showItem");
    emit operationSupportChanged("addItem");
    emit operationSupportChanged("editItem");
    emit operationSupportChanged("deleteItems");
}

bool DataInterfaceForge::prepareContextMenu(const Jsoner::Array &objects, QMenu *menu)
{
    if (objects.isEmpty())
        return false;

    DataInterface *interface = d_ptr->forgeInterface();

    if (d_ptr->showAction)
        d_ptr->showAction->setVisible(interface->isOperationSupported("showItem"));

    if (d_ptr->editAction)
        d_ptr->editAction->setVisible(interface->isOperationSupported("editItem"));

    if (d_ptr->deleteAction)
        d_ptr->deleteAction->setVisible(interface->isOperationSupported("deleteItems"));

    return true;
}

void DataInterfaceForge::fetchSearchSuggestions(const QString &query)
{
    if (!d_ptr->dataController)
        return;

    if (query.size() < 3)
        return;

    static QElapsedTimer timer;
    if (!timer.isValid())
        timer.start();
    else if (timer.elapsed() < 1000)
        return;

    DataGate::DataQuery dataQuery;
    dataQuery.setString(query);
    dataQuery = d_ptr->forgeInterface()->prepareQuery(dataQuery);

    QStringListModel *model = d_ptr->completionModel;

    d_ptr->dataController->fetchSearchSuggestions(dataQuery, [model](const DataGate::DataResponse &response) {
        const Jsoner::Array result = response.array();
        if (result.isEmpty())
            return;

        const QStringList current = model->stringList();
        QStringList newer;

        // Collect new suggestions, avoiding duplicates
        for (const QJsonValue &value : result) {
            const QString suggestion = value.toString();
            if (!current.contains(suggestion))
                newer.append(suggestion);
        }

        // Only update the model if there are new suggestions
        if (!newer.isEmpty()) {
            // Append the new suggestions to the current list
            QStringList updatedList = current;
            updatedList.append(newer);

            // Update the model with the new list
            model->setStringList(updatedList);
            //m_completer.complete();
        }

        timer.invalidate();
    });
}

/*!
 * \brief Process the data fetched using data controller's fetchObjects method
 * \param response the response object containing data
 */

void DataInterfaceForge::processFetchedData(const DataGate::DataResponse &response)
{
    if (response.hasMessage()) {
        QMessageBox box;
        box.setWindowTitle(response.isSuccess() ? tr("Message") : tr("Error"));
        box.setIcon(response.isSuccess() ? QMessageBox::Information : QMessageBox::Warning);
        box.setText(response.text());
        box.setInformativeText(response.informativeText());
        box.setDetailedText(response.detailedText());
        box.exec();
    }

    if (response.hasArray()) {
        d_ptr->tableModel->setArray(response.array());
    }

    if (response.hasPaginationData()) {
        ui->pageInput->setMaximum(response.pageCount());
        ui->pageInput->setValue(response.page());
        ui->pageInput->setSuffix(" / " + QString::number(response.pageCount()));

        ui->previousPageButton->setEnabled(response.page() > 1);
        ui->nextPageButton->setEnabled(response.page() < response.pageCount());
    }
}

void DataInterfaceForge::processOperationSupportChange(const QString &name, bool apply)
{
    if (name == "refresh") {
        ui->refreshButton->setEnabled(apply);
    }

    if (name == "search") {
        ui->searchInput->setVisible(apply);
    }

    if (name == "filter") {
        ui->filterButton->setVisible(apply);

        if (ui->filterContainer->isVisible() != apply)
            ui->filterContainer->setVisible(apply);
    }

    if (name == "addItem") {
        ui->addButton->setEnabled(apply);
    }

    if (name == "editItem") {
        ui->editButton->setEnabled(apply);
    }

    if (name == "deleteItems") {
        ui->deleteButton->setEnabled(apply);
    }
}

void DataInterfaceForge::init()
{
    ui = d_ptr->forgeInterface()->ui;

    initSearchCompletion();
    initFiltering();
    initButtons();
    initTable();
    initPagination();

    connect(d_ptr->forgeInterface(), &UserInterface::operationSupportChanged, this, &DataInterfaceForge::processOperationSupportChange);

    connect(this, &DataInterfaceForge::operationSupportChanged, this, [this](const QString &name) {
        processOperationSupportChange(name, d_ptr->forgeInterface()->isOperationSupported(name));
    });
}

void DataInterfaceForge::initSearchCompletion()
{
    d_ptr->completer = new QCompleter(d_ptr->forgeInterface());
    d_ptr->completer->setCaseSensitivity(Qt::CaseInsensitive);
    d_ptr->completer->setFilterMode(Qt::MatchContains);

    d_ptr->completionModel = new QStringListModel(this);
    d_ptr->completer->setModel(d_ptr->completionModel);

    ui->searchInput->setCompleter(d_ptr->completer);
    connect(ui->searchInput, &QLineEdit::textEdited, this, &DataInterfaceForge::fetchSearchSuggestions);
}

void DataInterfaceForge::initFiltering()
{
    connect(ui->toggleFiltersButtons, &QAbstractButton::toggled, ui->toggleFiltersButtons, [this](bool toggled) {
        ui->interfaceLayout->setHorizontalSpacing(toggled ? 6 : 0);
    });

    // Hiding filters at first show
    ui->toggleFiltersButtons->toggle();
    ui->toggleFiltersButtons->hide();
}

void DataInterfaceForge::initButtons()
{
    // Disabling CRUD buttons, will be enabled after edit factory setup
    ui->addButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
}

void DataInterfaceForge::initTable()
{
    DataGate::TableModel *model = new DataGate::TableModel(this);
    connect(model, &DataGate::TableModel::finished, this, &DataInterfaceForge::processFetchedData);

    ui->tableView->setModel(model);

    // Mouse shortcuts

    connect(ui->tableView, &QAbstractItemView::activated, ui->tableView, [this](const QModelIndex &index) {
        if (!d_ptr->forgeInterface()->isOperationSupported("showItem"))
            return;

        const Jsoner::Object object = d_ptr->tableModel->object(index.row());
        d_ptr->forgeInterface()->showItem(object);
    });

    // Reactions to model changes

    auto react = [this] {
        emit operationSupportChanged("showItem");
        emit operationSupportChanged("editItem");
        emit operationSupportChanged("deleteItems");
    };

    connect(model, &QAbstractItemModel::modelReset, model, [react, this] {
        react();
        QScrollBar *bar = ui->tableView->verticalScrollBar();
        bar->setValue(bar->minimum());
    });

    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    connect(selectionModel, &QItemSelectionModel::selectionChanged, selectionModel, [react](const QItemSelection &, const QItemSelection &) {
        react();
    });

    d_ptr->tableModel = model;
}

void DataInterfaceForge::initPagination()
{
    connect(ui->pageInput, &QSpinBox::valueChanged, this, [this](int page) {
        d_ptr->forgeInterface()->refresh();
    });
}

void DataInterfaceForge::setBluePrinted(bool printed)
{
    d_ptr->bluePrinted = printed;
}

} // namespace Widgetry
