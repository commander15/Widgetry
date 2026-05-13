#ifndef WIDGETRY_WIDGET_P_H
#define WIDGETRY_WIDGET_P_H

#include "widget.h"

#include <Widgetry/private/abstractwidget_p.h>

namespace Widgetry {

class WidgetPrivate : public AbstractWidgetPrivate
{
public:
    WidgetPrivate(Widget *q, const QByteArray &id);
    virtual ~WidgetPrivate() = default;
};

}

#endif // WIDGETRY_WIDGET_P_H
