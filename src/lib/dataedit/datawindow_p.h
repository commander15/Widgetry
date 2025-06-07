#ifndef WIDGETRY_DATAWINDOW_P_H
#define WIDGETRY_DATAWINDOW_P_H

#include "datawindow.h"

#include <Widgetry/private/widget_p.h>
#include <Widgetry/private/abstractdataedit_p.h>

namespace Widgetry {

class DataWindowPrivate : public WidgetPrivate
{
public:
    struct Edit {
        QString fieldName;
        AbstractDataEdit *edit = nullptr;
    };

    DataWindowPrivate(DataWindow *q, const QByteArray &id);

    AbstractDataEditPrivate *edit;
    QVector<Edit> edits;

    DataEditFinishedCallback addCallback;
    DataEditFinishedCallback editCallback;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAWINDOW_P_H
