#ifndef WIDGETRY_DATAWINDOW_P_H
#define WIDGETRY_DATAWINDOW_P_H

#include "datawindow.h"

#include <Widgetry/private/userinterface_p.h>
#include <Widgetry/private/abstractdataedit_p.h>

namespace Widgetry {

class DataWindowPrivate : public UserInterfacePrivate
{
public:
    struct Edit {
        QString fieldName;
        AbstractDataEdit *edit = nullptr;
    };

    DataWindowPrivate(DataWindow *q, const QByteArray &id);

    QVector<Edit> edits;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAWINDOW_P_H
