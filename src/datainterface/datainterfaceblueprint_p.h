#ifndef WIDGETRY_DATAINTERFACEBLUEPRINT_P_H
#define WIDGETRY_DATAINTERFACEBLUEPRINT_P_H

#include "datainterfaceblueprint.h"

namespace Widgetry {

class DataInterfaceForge;
class AbstractDataEditFactory;

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
    void buildInterface(bool init);
    void buildTable(bool init);
    void buildContextMenu(bool init);
    void buildEdit();
    void buildFilter();
    void buildDataController();

    void cleanup();

    QIcon interfaceIcon;
    QString interfaceTitle;
    QAction *interfaceAction = nullptr;

    bool searchAllowed = false;
    bool seachChanged = false;

    QList<QAbstractButton *> buttons;

    AbstractDataEdit *filter = nullptr;

    QStringList labels;
    QStringList fields;
    QList<QHeaderView::ResizeMode> resizeModes;
    QAbstractItemDelegate *delegate = nullptr;
    DataTableModel *model = nullptr;

    QMenu *contextMenu = nullptr;
    bool contextMenuAddDefaultActions = false;
    QList<Action> contextMenuActions;

    AbstractDataEditFactory *editFactory = nullptr;

    AbstractDataController *dataController = nullptr;

    bool commit = true;

    DataInterface *interface;
    DataInterfaceForge *forge;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACEBLUEPRINT_P_H
