#include "datawindow.h"
#include "datawindow_p.h"

namespace Widgetry {

DataWindow::DataWindow(const QByteArray &id, QWidget *parent, Qt::WindowFlags flags)
    : UserInterface(new DataWindowPrivate(this, id), parent, Qt::Window | flags)
{
}

DataWindow::~DataWindow()
{
}

QWidget *DataWindow::editWidget() const
{
    return const_cast<DataWindow *>(this);
}

void DataWindow::show()
{
    QWidget::show();
}

void DataWindow::show(const Jsoner::Object &object)
{
    setObject(object, ShowOperation);
    QWidget::show();
}

void DataWindow::add(const Jsoner::Object &object)
{
    setObject(object, AddOperation);
    QWidget::show();
}

void DataWindow::edit(const Jsoner::Object &object)
{
    setObject(object, EditOperation);
    QWidget::show();
}

void DataWindow::clear()
{
    AbstractDataEdit::clear();
    UserInterface::cleanupUi();
}

Widgetry::AbstractDataEdit::EditType DataWindow::editType() const
{
    return WindowEdit;
}

bool DataWindow::validateInput()
{
    return true;
}

DataWindowPrivate::DataWindowPrivate(DataWindow *q, const QByteArray &id)
    : UserInterfacePrivate(q, id)
{
}

} // namespace Widgetry
