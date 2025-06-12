#include "userinterface.h"

#include "userwindow.h"
#include "useredit.h"

#include <Jsoner/object.h>
#include <Jsoner/array.h>

#include <Widgetry/databrowserblueprint.h>
#include <Widgetry/dataselector.h>
#include <Widgetry/abstractdatabrowserhandler.h>

UserInterface::UserInterface(QWidget *parent)
    : Widgetry::DataBrowser("users", parent)
{
    Widgetry::DataBrowserBlueprint ui(this);

    ui.icon("/home/commander/Downloads/ChatGPT Image May 10, 2025, 02_34_14 PM.png");
    ui.title(tr("Users"));

    ui.column("id").label(tr("ID")).size(100);
    ui.column("name").label(tr("Name")).resize(QHeaderView::Stretch);
    ui.column("score").label(tr("Score")).resize(QHeaderView::ResizeToContents);

    ui.menuAction("actionTranslate", "Translate", this, &UserInterface::translate);

    ui.edit<UserWindow>().mainField("id").maxCount(2);
    ui.filter<UserEdit>()->registerFields();
}

UserInterface::~UserInterface()
{
}

void UserInterface::selectIt()
{
    Widgetry::DataSelector selector(this);
    selector.setWindowTitle(tr("User selector"));
    selector.setOption(Widgetry::DataSelector::SearchByText, false);
    selector.setOption(Widgetry::DataSelector::WithButtons, false);
    selector.setManager(dataManager());

    DataGate::DataRequest query = newRequest(Widgetry::AbstractRequestInterceptor::IndexRequest);
    query.setQuery("15");
    selector.setRequest(query);

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
        const QString field = fields.at(i);
        selector.setLabel(field, headers.at(i));
        selector.setResizeMode(field, QHeaderView::Stretch);
    }

    if (selector.exec()) {
        qDebug() << "Selection: " << selector.selectedObjects();
    } else {
        qDebug() << "No selection";
    }
}

void UserInterface::translate()
{
    Widgetry::DataBrowserBlueprint ui(this);
    ui.icon(":/widgetry/icons/widgetry.png");
    ui.column("id").label(tr("IDentifier")).size(300);
    ui.menuAction("actionTranslate", icon(), "Go for translation !");
    ui.menuAction("actionShow", icon(), tr("Open"));
    ui.button("refreshButton", icon(), "Yo !");
    ui.button("selectButton", QIcon("/home/commander/Projects/TMS/DEV/TMS/src/resources/icons/action_print.png"), this, &UserInterface::selectIt);
}
