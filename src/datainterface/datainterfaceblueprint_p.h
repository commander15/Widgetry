#ifndef WIDGETRY_DATAINTERFACEBLUEPRINT_P_H
#define WIDGETRY_DATAINTERFACEBLUEPRINT_P_H

#include "datainterfaceblueprint.h"

namespace Widgetry {

class DataInterfaceForge;

class DataInterfaceBlueprintPrivate
{
public:
    struct Action {
        QIcon icon;
        QString text;
        bool separator = false;
        QAction *action = nullptr;
    };

    DataInterfaceBlueprintPrivate(DataInterface *interface);

    bool build(bool init);
    void buildTable(bool init);
    void buildContextMenu(bool init);
    void buildEdit();
    void buildDataController();

    void cleanup();

    QWidget *filter = nullptr;

    QStringList labels;
    QStringList fields;
    QList<QHeaderView::ResizeMode> resizeModes;
    QAbstractItemDelegate *delegate = nullptr;
    Jsoner::TableModel *model = nullptr;

    QMenu *contextMenu = nullptr;
    bool contextMenuAddDefaultActions = false;
    QList<Action> contextMenuActions;

    DataEdit *edit = nullptr;
    bool editInDialog = false;

    AbstractDataController *dataController = nullptr;

    bool commit = true;

    DataInterface *interface;
    DataInterfaceForge *forge;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACEBLUEPRINT_P_H
