#include "datainterfacebase.h"
#include "datainterfacebase_p.h"

#include "ui_datainterface.h"

#include <Widgetry/dataedit.h>

#include <Jsoner/tablemodel.h>

#include <QtWidgets/qmenu.h>

namespace Widgetry {

DataInterfaceBase::DataInterfaceBase(Ui::DataInterface *ui, DataInterfaceBasePrivate *d)
    : ui(ui)
    , d(d)
{
}

DataInterfaceBase::~DataInterfaceBase()
{
}

QWidget *DataInterfaceBase::topBar() const
{
    return ui->topBar;
}

QLineEdit *DataInterfaceBase::searchBar() const
{
    return ui->searchInput;
}

QAbstractButton *DataInterfaceBase::filterToggleButton() const
{
    return ui->toggleFiltersButtons;
}

QAbstractButton *DataInterfaceBase::refreshButton() const
{
    return ui->refreshButton;
}

QAbstractButton *DataInterfaceBase::addButton() const
{
    return ui->addButton;
}

QAbstractButton *DataInterfaceBase::editButton() const
{
    return ui->editButton;
}

QAbstractButton *DataInterfaceBase::deleteButton() const
{
    return ui->deleteButton;
}

void DataInterfaceBase::addButton(QAbstractButton *button)
{
    ui->topBarLayout->addWidget(button);
}

void DataInterfaceBase::insertButton(QAbstractButton *before, QAbstractButton *button)
{
    int index = ui->topBarLayout->indexOf(before);
    ui->topBarLayout->insertWidget(index, button);
}

void DataInterfaceBase::removeButton(QAbstractButton *button)
{
    ui->topBarLayout->removeWidget(button);
}

QWidget *DataInterfaceBase::filterWidget() const
{
    return ui->filterContainer;
}

QAbstractButton *DataInterfaceBase::filterButton() const
{
    return ui->filterButton;
}

QTableView *DataInterfaceBase::tableView() const
{
    return ui->tableView;
}

Jsoner::TableModel *DataInterfaceBase::tableModel() const
{
    return static_cast<Jsoner::TableModel *>(ui->tableView->model());
}

QSpinBox *DataInterfaceBase::pageInput() const
{
    return ui->pageInput;
}

QAbstractButton *DataInterfaceBase::previousPageButton() const
{
    return ui->previousPageButton;
}

QAbstractButton *DataInterfaceBase::nextPageButton() const
{
    return ui->nextPageButton;
}

QMenu *DataInterfaceBase::contextMenu() const
{
    return d->contextMenu;
}

DataEdit *DataInterfaceBase::dataEdit() const
{
    return d->dataEdit;
}

Jsoner::Object DataInterfaceBase::currentObject() const
{
    const int index = ui->tableView->currentIndex().row();
    return (index >= 0 ? d->tableModel->object(index) : Jsoner::Object());
}

QList<Jsoner::Object> DataInterfaceBase::selectedObjects() const
{
    QItemSelectionModel *model = ui->tableView->selectionModel();
    const QModelIndexList indexes = (model ? model->selectedRows() : QModelIndexList());

    QList<Jsoner::Object> objects;
    for (const QModelIndex &index : indexes)
        objects.append(d->tableModel->object(index.row()));
    return objects;
}

void DataInterfaceBase::showObject(const Jsoner::Object &object)
{
    if (d->dataEdit) {
        d->dataEdit->show(object);
    }
}

void DataInterfaceBase::addObject(const Jsoner::Object &object)
{
    if (d->dataEdit)
        d->dataEdit->add(object);
}

void DataInterfaceBase::editObject(const Jsoner::Object &object)
{
    if (d->dataEdit)
        d->dataEdit->edit(object);
}

bool DataInterfaceBase::canDeleteObjects(const QList<Jsoner::Object> &objects)
{
    return !objects.isEmpty();
}

void DataInterfaceBase::deleteObjects(const QList<Jsoner::Object> &objects)
{
    const Jsoner::Array array = d->tableModel->array();
    for (const Jsoner::Object &object : objects)
        ;
}

void DataInterfaceBase::showContextMenu(const QList<Jsoner::Object> &objects, const QPoint &pos)
{
    if (d->contextMenu && prepareContextMenu(objects, d->contextMenu))
        d->contextMenu->popup(pos);
}

bool DataInterfaceBase::prepareContextMenu(const QList<Jsoner::Object> &objects, QMenu *menu)
{
    if (objects.isEmpty())
        return false;

    const bool single = objects.size() == 1;
    const bool multiple = objects.size() > 1;

    if (d->showAction)
        d->showAction->setVisible(single);

    if (d->editAction)
        d->editAction->setVisible(single);

    if (d->deleteAction)
        d->deleteAction->setVisible(multiple);

    return true;
}

void DataInterfaceBase::setFilterWidget(QWidget *widget)
{
    ui->filterContainerLayout->addWidget(widget);
    d->filterWidget = widget;
}

void DataInterfaceBase::setTableModel(Jsoner::TableModel *model)
{
    ui->tableView->setModel(model);
    d->tableModel = model;
}

void DataInterfaceBase::setDataEdit(DataEdit *edit)
{
    d->dataEdit = edit;
}

void DataInterfaceBase::setContextMenu(QMenu *menu, bool addDefaultActions)
{
    if (addDefaultActions) {
        d->showAction = new QAction(QIcon(":/icons/action_show.png"), QString(), ui->tableView);
        d->editAction = new QAction(ui->editButton->icon(), QString(), ui->tableView);
        d->deleteAction = new QAction(ui->deleteButton->icon(), QString(), ui->tableView);

        QList<QAction *> actions = menu->actions();
        QAction *first = (!actions.isEmpty() ? actions.first() : nullptr);

        actions = { d->showAction, d->editAction, d->deleteAction };
        if (first) {
            QAction *separator = new QAction();
            separator->setSeparator(true);
            actions.append(separator);
        }

        menu->insertActions(first, actions);
    }

    d->contextMenu = menu;
}

} // namespace Widgetry
