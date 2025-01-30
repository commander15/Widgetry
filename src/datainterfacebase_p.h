#ifndef WIDGETRY_DATAINTERFACEBASE_P_H
#define WIDGETRY_DATAINTERFACEBASE_P_H

#include "datainterfacebase.h"

namespace Widgetry {

class DataInterfaceBasePrivate
{
public:
    QWidget *filterWidget = nullptr;

    Jsoner::TableModel *tableModel = nullptr;

    QMenu *contextMenu = nullptr;
    QAction *showAction = nullptr;
    QAction *editAction = nullptr;
    QAction *deleteAction = nullptr;

    DataEdit *dataEdit = nullptr;
};

}

#endif // WIDGETRY_DATAINTERFACEBASE_P_H
