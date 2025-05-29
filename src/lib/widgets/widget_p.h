#ifndef WIDGETRY_WIDGET_P_H
#define WIDGETRY_WIDGET_P_H

#include "widget.h"

namespace Widgetry {

class WidgetPrivate
{
public:
    WidgetPrivate(Widget *q, const QByteArray &id);
    virtual ~WidgetPrivate() = default;

    Widget *q_ptr;

    QByteArray id;
    QAction *action;
};

}

#endif // WIDGETRY_WIDGET_P_H
