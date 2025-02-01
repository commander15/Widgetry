#include "datainterfaceforge.h"
#include "datainterfaceforge_p.h"

#include "ui_datainterface.h"

#include <Widgetry/datainterface.h>
#include <Widgetry/dataedit.h>

#include <Jsoner/tablemodel.h>

#include <QtWidgets/qmenu.h>
#include <QtWidgets/qdialog.h>

#include <QtCore/qitemselectionmodel.h>

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

QWidget *DataInterfaceForge::filterWidget() const
{
    return ui->filterContainer;
}

QAbstractButton *DataInterfaceForge::filterButton() const
{
    return ui->filterButton;
}

QTableView *DataInterfaceForge::tableView() const
{
    return ui->tableView;
}

Jsoner::TableModel *DataInterfaceForge::tableModel() const
{
    return static_cast<Jsoner::TableModel *>(ui->tableView->model());
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

DataEdit *DataInterfaceForge::dataEdit() const
{
    return d_ptr->dataEdit;
}

void DataInterfaceForge::setFilterWidget(QWidget *widget)
{
    ui->filterContainerLayout->addWidget(widget);
    d_ptr->filterWidget = widget;
}

void DataInterfaceForge::setTableModel(Jsoner::TableModel *model)
{
    ui->tableView->setModel(model);

    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    QObject::connect(selectionModel, &QItemSelectionModel::selectionChanged, selectionModel, [this](const QItemSelection &current, const QItemSelection &) {
        const bool single = current.count() == 1;
        const bool multiple = current.count() > 1;
        ui->editButton->setEnabled(single);
        ui->deleteButton->setEnabled(single && multiple);
    });

    d_ptr->tableModel = model;
}

void DataInterfaceForge::setDataEdit(DataEdit *edit)
{
    d_ptr->dataEdit = edit;
}

void DataInterfaceForge::setDataEdit(DataEdit *edit, QWidget *dialogParent)
{
    d_ptr->dataEdit = edit;
    if (dialogParent)
        d_ptr->dataEditDialog = DataEdit::dialogFromEdit(edit, dialogParent);
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
        d_ptr->deleteAction->setVisible(single && multiple);

    return true;
}

void DataInterfaceForge::init()
{
    ui = d_ptr->forgeInterface()->ui;
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
