#ifndef WIDGETRY_TABLEWIDGET_P_H
#define WIDGETRY_TABLEWIDGET_P_H

#include "tablewidget.h"

#include <Widgetry/private/widget_p.h>

namespace Widgetry {

class TableWidgetPrivate : public WidgetPrivate
{
public:
    TableWidgetPrivate(TableWidget *q, const QByteArray &id);

    void init(Ui::TableWidget *ui);

    QMenu *contextMenu;
    DataGate::TableModel *model;
};

}

#endif // WIDGETRY_TABLEWIDGET_P_H
