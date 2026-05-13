#ifndef WIDGETRY_DATAEDIT_P_H
#define WIDGETRY_DATAEDIT_P_H

#include "dataedit.h"

#include <Widgetry/private/widget_p.h>

namespace Widgetry {

class DataEditPrivate : public WidgetPrivate
{
public:
    explicit DataEditPrivate(DataEdit *q, const QByteArray &id);
    virtual ~DataEditPrivate() = default;

    AbstractDataEditPrivate *edit;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAEDIT_P_H
