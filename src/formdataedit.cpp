#include "formdataedit.h"
#include "formdataedit_p.h"

#include <QtWidgets/qformlayout.h>

namespace Widgetry {

FormDataEdit::FormDataEdit(QWidget *parent, Qt::WindowFlags flags)
    : DataEdit(new FormDataEditPrivate(this), parent, flags)
{
    WIDGETRY_D(FormDataEdit);
    d->createLayout();
}

FormDataEdit::~FormDataEdit()
{
}

bool FormDataEdit::isRowVisible(int row) const
{
    WIDGETRY_D(const FormDataEdit);
    if (row < 0 || row >= d->layout->rowCount())
        return false;
    return !d->layout->itemAt(row, QFormLayout::FieldRole)->widget()->isHidden();
}

void FormDataEdit::showRow(int row )
{
    WIDGETRY_D(FormDataEdit);
    if (row < 0 || row >= d->layout->rowCount())
        return;
    d->layout->itemAt(row, QFormLayout::FieldRole)->widget()->show();
}

void FormDataEdit::hideRow(int row )
{
    WIDGETRY_D(FormDataEdit);
    if (row < 0 || row >= d->layout->rowCount())
        return;
    d->layout->itemAt(row, QFormLayout::FieldRole)->widget()->hide();
}

void FormDataEdit::setRowVisible(int row, bool visible )
{
    WIDGETRY_D(FormDataEdit);
    if (row < 0 || row >= d->layout->rowCount())
        return;

    QWidget *widget = d->layout->itemAt(row, QFormLayout::FieldRole)->widget();
    if (widget->isHidden())
        widget->show();
    else
        widget->hide();
}

int FormDataEdit::addRow(const QString &label, QWidget *field )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->addRow(label, field);
    return d->layout->rowCount() - 1;
}

int FormDataEdit::addRow(const QString &label, QLayout *field )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->addRow(label, field);
    return d->layout->rowCount() - 1;
}

int FormDataEdit::addRow(QWidget *label, QLayout *field )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->addRow(label, field);
    return d->layout->rowCount() - 1;
}

int FormDataEdit::addRow(QWidget *label, QWidget *field )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->addRow(label, field);
    return d->layout->rowCount() - 1;
}

int FormDataEdit::addRow(QWidget *widget )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->addRow(widget);
    return d->layout->rowCount() - 1;
}

int FormDataEdit::addRow(QLayout *layout )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->addRow(layout);
    return d->layout->rowCount() - 1;
}

void FormDataEdit::insertRow(int row, const QString &label, QWidget *field )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->insertRow(row, label, field);
}

void FormDataEdit::insertRow(int row, const QString &label, QLayout *field )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->insertRow(row, label, field);
}

void FormDataEdit::insertRow(int row, QWidget *label, QLayout *field )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->insertRow(row, label, field);
}

void FormDataEdit::insertRow(int row, QWidget *label, QWidget *field )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->insertRow(row, label, field);
}

void FormDataEdit::insertRow(int row, QWidget *widget )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->insertRow(row, widget);
}

void FormDataEdit::insertRow(int row, QLayout *layout )
{
    WIDGETRY_D(FormDataEdit);
    d->layout->insertRow(row, layout);
}

void FormDataEdit::removeRow(int row )
{
    WIDGETRY_D(FormDataEdit);
    if (row < 0 || row >= d->layout->rowCount())
        return;

    d->layout->removeRow(row);
}

int FormDataEdit::rowCount() const
{
    WIDGETRY_D(FormDataEdit);
    return d->layout->rowCount();
}

QFormLayout *FormDataEdit::layout() const
{
    WIDGETRY_D(const FormDataEdit);
    return d->layout;
}

void FormDataEdit::clear()
{
    DataEdit::clear();
}

void FormDataEdit::render(const Jsoner::Object &object, Operation operation)
{
    //
}

void FormDataEdit::extract(Jsoner::Object &object, Operation operation) const
{
    //
}

void FormDataEdit::makeWriteable(bool writeable)
{
    //
}

FormDataEditPrivate::FormDataEditPrivate(FormDataEdit *edit)
    : DataEditPrivate(edit)
    , layout(nullptr)
{
}

void FormDataEditPrivate::createLayout()
{
    layout = new QFormLayout(q_ptr);
}

} // namespace Widgetry
