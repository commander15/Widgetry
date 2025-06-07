#include "userinterface.h"

#include "userwindow.h"
#include "useredit.h"

#include "datacontroller.h"

#include <Widgetry/databrowserblueprint.h>
#include <Widgetry/dataselector.h>

UserInterface::UserInterface(QWidget *parent)
    : Widgetry::DataBrowser("users", parent)
{
    Widgetry::DataBrowserBlueprint ui(this);

    ui.title(tr("Users"));

    ui.button("selectButton", QIcon("/home/commander/Projects/TMS/DEV/TMS/src/resources/icons/action_print.png"), this, &UserInterface::selectIt);

    ui.tableColumn(tr("ID"), "id", QHeaderView::ResizeToContents);
    ui.tableColumn(tr("Name"), "name", QHeaderView::Stretch);
    ui.tableColumn(tr("Score"), "score", QHeaderView::ResizeToContents);

    ui.edit<UserWindow>()->mainField("id")->maxCount(2);
    ui.filter<UserEdit>()->registerFields();

    ui.contextMenuAction("Hello", this, &UserInterface::selectIt);

    ui.dataManager<DataController>();
}

UserInterface::~UserInterface()
{
}

void UserInterface::selectIt()
{
    Widgetry::DataSelector selector(this);
    selector.setWindowTitle(tr("User selector"));
    selector.setDataManager(dataManager());

    DataGate::DataQuery query = newQuery(RefreshQuery);
    selector.setSearchQuery(query);

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
        selector.setLabel(i, headers.at(i));
        selector.setResizeMode(i, QHeaderView::Stretch);
    }

    if (selector.exec()) {
        qDebug() << "Selection: " << selector.selectedObjects();
    } else {
        qDebug() << "No selection";
    }
}
