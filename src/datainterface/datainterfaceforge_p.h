#ifndef WIDGETRY_DATAINTERFACEBASE_P_H
#define WIDGETRY_DATAINTERFACEBASE_P_H

#include "datainterfaceforge.h"

namespace Widgetry {

class DataInterface;

class DataInterfaceForgePrivate
{
public:
    virtual ~DataInterfaceForgePrivate() = default;

    virtual DataInterface *forgeInterface() const = 0;

    QWidget *filterWidget = nullptr;

    Jsoner::TableModel *tableModel = nullptr;

    QMenu *contextMenu = nullptr;
    QAction *showAction = nullptr;
    QAction *editAction = nullptr;
    QAction *deleteAction = nullptr;

    AbstractDataEdit *dataEdit = nullptr;
    QDialog *dataEditDialog = nullptr;

    bool bluePrinted = false;
};

}

#endif // WIDGETRY_DATAINTERFACEBASE_P_H
