#include "userinterface.h"

#include "useredit.h"

#include "datacontroller.h"

#include <Widgetry/datainterfaceblueprint.h>

UserInterface::UserInterface(QWidget *parent)
    : Widgetry::DataInterface("users", parent)
{
    Widgetry::DataInterfaceBlueprint ui(this);

    ui.title(tr("Users"));

    ui.button(QIcon("/home/commander/Projects/TMS/DEV/TMS/src/resources/icons/action_print.png"), this, &UserInterface::refresh);

    ui.tableHeader(tr("ID"), "id", QHeaderView::ResizeToContents);
    ui.tableHeader(tr("Name"), "name", QHeaderView::Stretch);
    ui.tableHeader(tr("Score"), "score", QHeaderView::ResizeToContents);

    ui.search();
    ui.edit<UserEdit>()->mainField("id")->maxCount(1);
    ui.filter<UserEdit>();

    ui.contextMenu();
    ui.contextMenuAction("Hello", this, &UserInterface::addItem);

    ui.dataController<DataController>();
}

UserInterface::~UserInterface()
{
}
