#ifndef WIDGETRY_DATAWINDOW_P_H
#define WIDGETRY_DATAWINDOW_P_H

#include "datawindow.h"

#include <Widgetry/private/dataedit_p.h>

namespace Widgetry {

class DataWindowPrivate : public DataEditPrivate
{
public:
    struct Edit {
        QString fieldName;
        AbstractDataEdit *edit = nullptr;
    };

    DataWindowPrivate(DataWindow *q, const QByteArray &id);

    QVector<Edit> edits;
    bool browserSet;

    DataEditFinishedCallback addCallback;
    DataEditFinishedCallback editCallback;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAWINDOW_P_H
