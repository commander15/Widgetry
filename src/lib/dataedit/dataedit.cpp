#include "dataedit.h"
#include "dataedit_p.h"

namespace Widgetry {

DataEdit::DataEdit(QWidget *parent, Qt::WindowFlags flags)
    : DataEdit(new DataEditPrivate(this, QByteArray()), parent, flags)
{
}

DataEdit::DataEdit(DataEditPrivate *d, QWidget *parent, Qt::WindowFlags flags)
    : Widgetry::Widget(d, parent, flags)
    , AbstractDataEdit(this)
{
    bindEnd(this, &DataEdit::editingFinished);
    d->edit = AbstractDataEdit::d_ptr.get();
}

DataEdit::~DataEdit()
{
}

void DataEdit::render(const Jsoner::Object &object)
{
    Q_UNUSED(object);
}

void DataEdit::extract(Jsoner::Object &object) const
{
    Q_UNUSED(object);
}

void DataEdit::makeVisible(bool visible, int result)
{
    if (visible && !isVisible()) {
        show();
        return;
    }

    if (!visible && isVisible()) {
        hide();
        emit editingFinished(result);
    }
}

DataEditPrivate::DataEditPrivate(DataEdit *q, const QByteArray &id)
    : WidgetPrivate(q, id)
    , edit(nullptr)
{
}


} // namespace Widgetry
