#include "userinterface.h"

#include "userwindow.h"
#include "useredit.h"

#include "datacontroller.h"

#include <Widgetry/datainterfaceblueprint.h>
#include <Widgetry/dataselector.h>

UserInterface::UserInterface(QWidget *parent)
    : Widgetry::DataInterface("users", parent)
{
    Widgetry::DataInterfaceBlueprint ui(this);

    ui.title(tr("Users"));

    ui.button(QIcon("/home/commander/Projects/TMS/DEV/TMS/src/resources/icons/action_print.png"), this, &UserInterface::selectIt);

    ui.tableHeader(tr("ID"), "id", QHeaderView::ResizeToContents);
    ui.tableHeader(tr("Name"), "name", QHeaderView::Stretch);
    ui.tableHeader(tr("Score"), "score", QHeaderView::ResizeToContents);

    ui.search();
    ui.edit<UserEdit>()->mainField("id")->maxCount(2);
    ui.filter<UserEdit>();

    ui.contextMenu();
    ui.contextMenuAction("Hello", this, &UserInterface::addNewItem);

    ui.dataController<DataController>();
}

UserInterface::~UserInterface()
{
}

void UserInterface::selectIt()
{
    operate("editItem");
    return;

    Widgetry::DataSelector selector(this);
    selector.setWindowTitle(tr("User selector"));
    selector.setDataController(dataController());

    QStringList headers;
    QStringList fields;
    auto field = [&headers, &fields](const QString &label, const QString &name) {
        headers.append(label);
        fields.append(name);
    };

    field(tr("Name"), "name");
    field(tr("Score"), "score");

    selector.setFields(fields);
    for (int i(0); i < headers.size(); ++i) {
        selector.setHeader(i, headers.at(i));
        selector.setResizeMode(i, QHeaderView::Stretch);
    }

    if (selector.exec()) {
        qDebug() << "Selection: " << selector.selectedObjects();
    } else {
        qDebug() << "No selection";
    }
}
