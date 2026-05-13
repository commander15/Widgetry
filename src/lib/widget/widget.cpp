#include "widget.h"
#include "widget_p.h"

namespace Widgetry {

Widget::Widget(const QByteArray &id, QWidget *parent, Qt::WindowFlags flags)
    : Widget(new WidgetPrivate(this, id), parent, flags)
{
}

Widget::Widget(QWidget *parent, Qt::WindowFlags flags)
    : Widget(new WidgetPrivate(this, QByteArray()), parent, flags)
{
}

Widget::Widget(WidgetPrivate *d, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
    , AbstractWidget(d)
{
    d->initWidget<Widget>();
}

Widget::~Widget()
{
}

void Widget::sync()
{
    AbstractWidget::sync();
}

void Widget::processIconChange(const QIcon &icon)
{
    emit iconChanged(icon);
}

void Widget::processTitleChange(const QString &title)
{
    emit titleChanged(title);
}

void Widget::processActionChange(QAction *current, QAction *previous)
{
    if (previous) {
        disconnect(this, &Widget::iconChanged, previous, &QAction::setIcon);
        disconnect(this, &Widget::titleChanged, previous, &QAction::setText);
    }

    if (current) {
        setIcon(current->icon());
        setTitle(current->text());
        connect(this, &Widget::iconChanged, current, &QAction::setIcon);
        connect(this, &Widget::titleChanged, current, &QAction::setText);
    }
}

void Widget::processOperationSupportChanged(const QString &operation, bool supported)
{
    emit operationSupportChanged(operation, supported);
}

bool Widget::event(QEvent *event)
{
    d_ptr->processEvent(event);
    return QWidget::event(event);
}

WidgetPrivate::WidgetPrivate(Widget *q, const QByteArray &id)
    : AbstractWidgetPrivate(q, q)
{
    this->id = id;
}

} // namespace Widgetry
