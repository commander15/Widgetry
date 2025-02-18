#include "datainterfaceblueprint.h"
#include "datainterfaceblueprint_p.h"

#include <Widgetry/datainterface.h>
#include <Widgetry/datainterfaceforge.h>

#include <Jsoner/tablemodel.h>

#include <QtWidgets/qtableview.h>
#include <QtWidgets/qmenu.h>

namespace Widgetry {

DataInterfaceBlueprint::DataInterfaceBlueprint(DataInterface *interface)
    : d_ptr(new DataInterfaceBlueprintPrivate(interface))
{
}

DataInterfaceBlueprint::~DataInterfaceBlueprint()
{
    if (d_ptr->build(isInit()))
        if (!d_ptr->forge->isBluePrinted())
            d_ptr->forge->setBluePrinted();
}

void DataInterfaceBlueprint::filter(QWidget *filter)
{
    d_ptr->filter = filter;
}

int DataInterfaceBlueprint::tableHeader(const QString &label)
{
    return tableHeader(label, QString(), QHeaderView::Custom);
}

int DataInterfaceBlueprint::tableHeader(const QString &label, const QString &field, QHeaderView::ResizeMode resizeMode)
{
    d_ptr->labels.append(label);
    d_ptr->fields.append(field);
    d_ptr->resizeModes.append(resizeMode);
    return d_ptr->fields.count() - 1;
}

int DataInterfaceBlueprint::tableHeader(const QString &field, QHeaderView::ResizeMode resizeMode)
{
    return tableHeader(QString(), field, resizeMode);
}

void DataInterfaceBlueprint::tableDelegate(QAbstractItemDelegate *delegate)
{
    d_ptr->delegate = delegate;
}

void DataInterfaceBlueprint::tableModel(Jsoner::TableModel *model)
{
    d_ptr->model = model;
}

QMenu *DataInterfaceBlueprint::contextMenu(bool addDefaultActions)
{
    QMenu *menu = new QMenu(d_ptr->interface);
    contextMenu(menu, addDefaultActions);
    return menu;
}

void DataInterfaceBlueprint::contextMenu(QMenu *menu, bool addDefaultActions)
{
    d_ptr->contextMenu = menu;
    d_ptr->contextMenuAddDefaultActions = addDefaultActions;
}

QAction *DataInterfaceBlueprint::contextMenuAction(const QIcon &icon)
{
    DataInterfaceBlueprintPrivate::Action actionPrint;
    actionPrint.icon = icon;
    if (isInit())
        actionPrint.action = (new QAction(icon, QString(), d_ptr->interface));
    d_ptr->contextMenuActions.append(actionPrint);
    return actionPrint.action;
}

QAction *DataInterfaceBlueprint::contextMenuAction(const QString &text)
{
    DataInterfaceBlueprintPrivate::Action actionPrint;
    actionPrint.text = text;
    if (isInit())
        actionPrint.action = (new QAction(text, d_ptr->interface));
    d_ptr->contextMenuActions.append(actionPrint);
    return actionPrint.action;
}

QAction *DataInterfaceBlueprint::contextMenuAction(const QIcon &icon, const QString &text)
{
    DataInterfaceBlueprintPrivate::Action actionPrint;
    actionPrint.icon = icon;
    actionPrint.text = text;
    if (isInit())
        actionPrint.action = (new QAction(icon, text, d_ptr->interface));
    d_ptr->contextMenuActions.append(actionPrint);
    return actionPrint.action;
}

void DataInterfaceBlueprint::contextMenuAction(QAction *action)
{
    DataInterfaceBlueprintPrivate::Action actionPrint;
    actionPrint.action = action;
    d_ptr->contextMenuActions.append(actionPrint);
}

QAction *DataInterfaceBlueprint::contextMenuSeparator()
{
    DataInterfaceBlueprintPrivate::Action actionPrint;
    actionPrint.separator = true;
    if (isInit()) {
        actionPrint.action = new QAction(d_ptr->interface);
        actionPrint.action->setSeparator(true);
    }
    d_ptr->contextMenuActions.append(actionPrint);
    return actionPrint.action;
}

void DataInterfaceBlueprint::edit(DataEdit *edit, bool putOnDialog)
{
    d_ptr->edit = edit;
    d_ptr->editInDialog = putOnDialog;
}

void DataInterfaceBlueprint::dataController(AbstractDataController *controller)
{
    d_ptr->dataController = controller;
}

DataInterface *DataInterfaceBlueprint::interface() const
{
    return d_ptr->interface;
}

bool DataInterfaceBlueprint::isInit() const
{
    return !d_ptr->forge->isBluePrinted();
}

DataInterfaceBlueprintPrivate::DataInterfaceBlueprintPrivate(DataInterface *interface)
    : interface(interface)
    , forge(interface->forge())
{
}

bool DataInterfaceBlueprintPrivate::build(bool init)
{
    if (commit) {
        if (init) {
            buildTable(init);
            buildContextMenu(init);
        }

        buildEdit();
        buildDataController();
        return true;
    } else {
        cleanup();
        return false;
    }
}

void DataInterfaceBlueprintPrivate::buildTable(bool init)
{
    QTableView *tableView = forge->tableView();

    if (delegate)
        forge->tableView()->setItemDelegate(delegate);

    Jsoner::TableModel *model;
    if (init) {
        model = (this->model ? this->model : new Jsoner::TableModel(interface));
        model->setFields(fields);
        forge->setTableModel(model);
    } else {
        model = forge->tableModel();
    }

    QHeaderView *header = forge->tableView()->horizontalHeader();
    for (int i(0); i < fields.size(); ++i) {
        if (init)
            header->setSectionResizeMode(i, resizeModes.at(i));

        const QString label = labels.at(i);
        model->setHeaderData(i, Qt::Horizontal, (!label.isEmpty() ? label : fields.at(i)));
    }
}

void DataInterfaceBlueprintPrivate::buildContextMenu(bool init)
{
    if (init) {
        if (!contextMenu)
            return;

        forge->setContextMenu(contextMenu, contextMenuAddDefaultActions);

        QList<QAction *> actions;
        actions.resize(contextMenuActions.size());
        std::transform(contextMenuActions.begin(), contextMenuActions.end(), actions.begin(), [](const Action &action) {
            return action.action;
        });

        if (!actions.isEmpty()) {
            if (contextMenuAddDefaultActions)
                contextMenu->addSeparator();
            contextMenu->addActions(actions);
        }
    } else {
        contextMenu = forge->contextMenu();
        if (!contextMenu)
            return;

        int startIndex = 0;
        const QList<QAction *> actions = contextMenu->actions();

        for (int i(0); i < contextMenuActions.size(); ++i) {
            const int actionIndex = startIndex + i;
            if (actionIndex >= actions.size())
                continue;

            const Action actionPrint = contextMenuActions.at(i);
            QAction *action = actions.at(actionIndex);
            action->setText(actionPrint.text);
        }
    }
}

void DataInterfaceBlueprintPrivate::buildEdit()
{
    if (!edit)
        return;

    if (editInDialog)
        forge->setDataEdit(edit, interface->window());
    else
        forge->setDataEdit(edit);
}

void DataInterfaceBlueprintPrivate::buildDataController()
{
    if (!dataController)
        return;

    interface->setDataController(dataController);
}

void DataInterfaceBlueprintPrivate::cleanup()
{
    //
}

} // namespace Widgetry
