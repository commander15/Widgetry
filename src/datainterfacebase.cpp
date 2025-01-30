#include "datainterfacebase.h"

#include "ui_datainterface.h"

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

} // namespace Widgetry
