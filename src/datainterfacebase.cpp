#include "datainterfacebase.h"
#include "datainterfacebase_p.h"

#include "ui_datainterface.h"

#include <Widgetry/dataedit.h>

#include <Jsoner/tablemodel.h>

#include <QtWidgets/qmenu.h>
#include <QtWidgets/qdialog.h>

namespace Widgetry {

DataInterfaceBase::DataInterfaceBase(Ui::DataInterface *ui, DataInterfaceBasePrivate *d)
    : ui(ui)
    , data(d)
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
    return data->contextMenu;
}

DataEdit *DataInterfaceBase::dataEdit() const
{
    return data->dataEdit;
}

Jsoner::Object DataInterfaceBase::currentObject() const
{
    const int index = ui->tableView->currentIndex().row();
    return (index >= 0 ? data->tableModel->object(index) : Jsoner::Object());
}

Jsoner::Array DataInterfaceBase::selectedObjects() const
{
    QItemSelectionModel *model = ui->tableView->selectionModel();
    const QModelIndexList indexes = (model ? model->selectedRows() : QModelIndexList());

    Jsoner::Array objects;
    for (const QModelIndex &index : indexes)
        objects.append(data->tableModel->object(index.row()));
    return objects;
}

void DataInterfaceBase::showObject(const Jsoner::Object &object)
{
    if (!data->dataEdit)
        return;

    data->dataEdit->show(object);
}

Jsoner::Object DataInterfaceBase::addObject(const Jsoner::Object &object)
{
    if (!data->dataEdit)
        return Jsoner::Object();

    data->dataEdit->add(object);

    if (!data->dataEditDialog || !data->dataEditDialog->exec())
        return Jsoner::Object();

    return (data->dataEdit->isComplete() ? data->dataEdit->object() : Jsoner::Object());
}

Jsoner::Object DataInterfaceBase::editObject(const Jsoner::Object &object)
{
    if (!data->dataEdit)
        return Jsoner::Object();

    data->dataEdit->edit(object);

    if (!data->dataEditDialog || !data->dataEditDialog->exec())
        return Jsoner::Object();

    return (data->dataEdit->isComplete() ? data->dataEdit->object() : Jsoner::Object());
}

void DataInterfaceBase::showContextMenu(const QList<Jsoner::Object> &objects, const QPoint &pos)
{
    if (data->contextMenu && prepareContextMenu(objects, data->contextMenu))
        data->contextMenu->popup(pos);
}

bool DataInterfaceBase::prepareContextMenu(const QList<Jsoner::Object> &objects, QMenu *menu)
{
    if (objects.isEmpty())
        return false;

    const bool single = objects.size() == 1;
    const bool multiple = objects.size() > 1;

    if (data->showAction)
        data->showAction->setVisible(single);

    if (data->editAction)
        data->editAction->setVisible(single);

    if (data->deleteAction)
        data->deleteAction->setVisible(multiple);

    return true;
}

void DataInterfaceBase::setFilterWidget(QWidget *widget)
{
    ui->filterContainerLayout->addWidget(widget);
    data->filterWidget = widget;
}

void DataInterfaceBase::setTableModel(Jsoner::TableModel *model)
{
    ui->tableView->setModel(model);
    data->tableModel = model;
}

void DataInterfaceBase::setDataEdit(DataEdit *edit)
{
    data->dataEdit = edit;
}

void DataInterfaceBase::setDataEdit(DataEdit *edit, QWidget *dialogParent)
{
    data->dataEdit = edit;
    if (dialogParent)
        data->dataEditDialog = DataEdit::dialogFromEdit(edit, dialogParent);
}

void DataInterfaceBase::setContextMenu(QMenu *menu, bool addDefaultActions)
{
    if (addDefaultActions) {
        data->showAction = new QAction(QIcon(":/icons/action_show.png"), QString(), ui->tableView);
        data->editAction = new QAction(ui->editButton->icon(), QString(), ui->tableView);
        data->deleteAction = new QAction(ui->deleteButton->icon(), QString(), ui->tableView);

        QList<QAction *> actions = menu->actions();
        QAction *first = (!actions.isEmpty() ? actions.first() : nullptr);

        actions = { data->showAction, data->editAction, data->deleteAction };
        if (first) {
            QAction *separator = new QAction();
            separator->setSeparator(true);
            actions.append(separator);
        }

        menu->insertActions(first, actions);
    }

    data->contextMenu = menu;
}

} // namespace Widgetry
