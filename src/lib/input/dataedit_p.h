#ifndef WIDGETRY_DATAEDIT_P_H
#define WIDGETRY_DATAEDIT_P_H

#include "dataedit.h"

#include <Widgetry/private/abstractdataedit_p.h>

namespace Widgetry {

class DataEditPrivate : public AbstractDataEditPrivate
{
public:
    explicit DataEditPrivate(DataEdit *q);
    virtual ~DataEditPrivate() = default;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAEDIT_P_H
