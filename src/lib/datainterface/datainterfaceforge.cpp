#include "datainterfaceforge.h"
#include "datainterfaceforge_p.h"

#include "ui_datainterface.h"

#include <Widgetry/datainterface.h>
#include <Widgetry/dataedit.h>

#include <DataGate/tablemodel.h>

#include <QtWidgets/qmenu.h>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qscrollbar.h>
#include <QtWidgets/qabstractitemview.h>
#include <QtWidgets/qcompleter.h>

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
    return d_ptr->tableModel;
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

void DataInterfaceForge::setFilterWidget(AbstractDataEdit *widget)
{
    ui->filterContainerLayout->addWidget(widget->editWidget());
    ui->toggleFiltersButtons->setVisible(widget);
    d_ptr->filterWidget = widget;
}

void DataInterfaceForge::setTableModel(DataGate::TableModel *model)
{
    ui->tableView->setModel(model);

    auto updateButtons = [this](int selectedRows) {
        const bool single = selectedRows == 1;
        const bool multiple = selectedRows > 1;
        ui->editButton->setEnabled(single);
        ui->deleteButton->setEnabled(single || multiple);
    };

    auto updateOperations = [this](int selectedRows) {
        d_ptr->forgeInterface()->sync();
    };

    QObject::connect(model, &QAbstractItemModel::modelReset, model, [updateButtons, updateOperations, this] {
        updateButtons(0);
        updateOperations(0);

        QScrollBar *bar = ui->tableView->verticalScrollBar();
        bar->setValue(bar->minimum());
    });

    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    QObject::connect(selectionModel, &QItemSelectionModel::selectionChanged, selectionModel, [updateButtons, updateOperations, selectionModel](const QItemSelection &, const QItemSelection &) {
        updateButtons(selectionModel->selectedRows().count());
        updateOperations(selectionModel->selectedRows().count());
    });

    ui->refreshButton->setEnabled(model);

    d_ptr->tableModel = model;
}

void DataInterfaceForge::setDataEdit(AbstractDataEditFactory *factory)
{
    d_ptr->dataEditFactory = factory;

    ui->addButton->setEnabled(factory);
    ui->editButton->setEnabled(factory);
    ui->deleteButton->setEnabled(factory);
}

void DataInterfaceForge::setContextMenu(QMenu *menu, bool addDefaultActions)
{
    if (addDefaultActions) {
        d_ptr->showAction = new QAction(QIcon(":/icons/action_show.png"), QString(), menu);
        QObject::connect(d_ptr->showAction, &QAction::triggered, d_ptr->forgeInterface(), &DataInterface::showCurrentItem);

        d_ptr->editAction = new QAction(ui->editButton->icon(), QString(), menu);
        QObject::connect(d_ptr->editAction, &QAction::triggered, d_ptr->forgeInterface(), &DataInterface::editCurrentItem);

        d_ptr->deleteAction = new QAction(ui->deleteButton->icon(), QString(), menu);
        QObject::connect(d_ptr->deleteAction, &QAction::triggered, d_ptr->forgeInterface(), &DataInterface::deleteSelectedItems);

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

    QObject::connect(ui->tableView, &QWidget::customContextMenuRequested, menu, [this, menu](const QPoint &pos) {
        DataInterface *interface = d_ptr->forgeInterface();;
        interface->showContextMenu(interface->selectedObjects(), pos);
    });

    d_ptr->contextMenu = menu;
}

bool DataInterfaceForge::prepareContextMenu(const Jsoner::Array &objects, QMenu *menu)
{
    if (objects.isEmpty())
        return false;

    const bool single = objects.size() == 1;
    const bool multiple = objects.size() > 1;

    if (d_ptr->showAction)
        d_ptr->showAction->setVisible(single);

    if (d_ptr->editAction)
        d_ptr->editAction->setVisible(single);

    if (d_ptr->deleteAction)
        d_ptr->deleteAction->setVisible(single || multiple);

    return true;
}

void DataInterfaceForge::init()
{
    ui = d_ptr->forgeInterface()->ui;

    d_ptr->completionModel = new QStringListModel(this);

    d_ptr->completer = new QCompleter(d_ptr->forgeInterface());
    d_ptr->completer->setCaseSensitivity(Qt::CaseInsensitive);
    d_ptr->completer->setFilterMode(Qt::MatchContains);
    d_ptr->completer->setModel(d_ptr->completionModel);

    ui->searchInput->setEnabled(false);
    ui->searchInput->setCompleter(d_ptr->completer);

    QObject::connect(ui->searchInput, &QLineEdit::textChanged, d_ptr->forgeInterface(), &DataInterface::fetchSearchSuggestions);

    QObject::connect(ui->toggleFiltersButtons, &QAbstractButton::toggled, ui->toggleFiltersButtons, [this](bool toggled) {
        ui->interfaceLayout->setHorizontalSpacing(toggled ? 6 : 0);
    });
    ui->toggleFiltersButtons->toggle();
    ui->toggleFiltersButtons->hide();

    ui->refreshButton->setEnabled(false);
    ui->addButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);

    QObject::connect(ui->tableView, &QAbstractItemView::activated, ui->tableView, [this](const QModelIndex &index) {
        if (!d_ptr->tableModel)
            return;

        d_ptr->forgeInterface()->showCurrentItem();
    });

    QObject::connect(ui->pageInput, &QSpinBox::valueChanged, ui->pageInput, [this](int value) {
        d_ptr->forgeInterface()->refresh();
    });
}

bool DataInterfaceForge::isBluePrinted() const
{
    return d_ptr->bluePrinted;
}

void DataInterfaceForge::setBluePrinted(bool printed)
{
    d_ptr->bluePrinted = printed;
}

} // namespace Widgetry
