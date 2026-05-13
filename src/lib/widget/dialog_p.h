#ifndef WIDGETRY_DIALOG_P_H
#define WIDGETRY_DIALOG_P_H

#include "dialog.h"

#include <Widgetry/private/widget_p.h>

namespace Widgetry {

class DialogPrivate : public AbstractWidgetPrivate
{
public:
    explicit DialogPrivate(Dialog *q, const QByteArray &id);
    virtual ~DialogPrivate() = default;
};

}

#endif // WIDGETRY_DIALOG_P_H
