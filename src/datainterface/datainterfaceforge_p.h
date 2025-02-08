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

    AbstractDataEdit *filterWidget = nullptr;

    DataTableModel *tableModel = nullptr;

    QMenu *contextMenu = nullptr;
    QAction *showAction = nullptr;
    QAction *editAction = nullptr;
    QAction *deleteAction = nullptr;

    AbstractDataEditFactory *dataEditFactory = nullptr;

    bool bluePrinted = false;
};

}

#endif // WIDGETRY_DATAINTERFACEBASE_P_H
