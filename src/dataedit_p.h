#ifndef WIDGETRY_DATAEDIT_P_H
#define WIDGETRY_DATAEDIT_P_H

#include "dataedit.h"

namespace Widgetry {

class DataEditPrivate
{
public:
    DataEditPrivate(DataEdit *q);

    DataEdit *q_ptr;

    Jsoner::Object object;
    DataEdit::Operation operation;
    bool readOnly;

    QString completionErrorString;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAEDIT_P_H
