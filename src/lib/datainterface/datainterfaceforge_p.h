#ifndef WIDGETRY_DATAINTERFACEBASE_P_H
#define WIDGETRY_DATAINTERFACEBASE_P_H

#include "datainterfaceforge.h"

#include <DataGate/abstractdatacontroller.h>

class QStringListModel;

namespace Widgetry {

class DataInterface;

class DataInterfaceForgePrivate
{
public:
    virtual ~DataInterfaceForgePrivate() = default;

    virtual DataInterface *forgeInterface() const = 0;

    QCompleter *completer = nullptr;
    QStringListModel *completionModel = nullptr;

    AbstractDataEdit *filterWidget = nullptr;

    DataGate::TableModel *tableModel = nullptr;

    QMenu *contextMenu = nullptr;
    QAction *showAction = nullptr;
    QAction *editAction = nullptr;
    QAction *deleteAction = nullptr;

    AbstractDataEditFactory *dataEditFactory = nullptr;

    DataGate::AbstractDataController *dataController = nullptr;

    bool bluePrinted = false;
};

}

#endif // WIDGETRY_DATAINTERFACEBASE_P_H
