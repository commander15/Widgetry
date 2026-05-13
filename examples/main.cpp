#include <QApplication>
#include <QDialog>

#include "logincontroller.h"
#include "datacontroller.h"
#include "mainwindow.h"
#include "useredit.h"
#include "userwindow.h"

#include <Widgetry/loadindicator.h>

#include <Widgetry/widgetry.h>
#include <Widgetry/databrowserblueprint.h>

int main(int argc, char *argv[])
{
    Widgetry::Application app(argc, argv);

    LoginController logins;
    app.setLoginManager(&logins);

    DataController data;
    app.setDataManager(&data);

    Widgetry::LoginDialog login;
    login.setIdentifierType(Widgetry::LoginDialog::UsernameIdentifier);
    login.showOnLogOut();
    app.setLoginDialog(&login);

    Widgetry::DataBrowser browser;
    app.setMainWidget(&browser);

    if (true) {
        auto tr = [](const char *msg) { return msg; };

        Widgetry::DataBrowserBlueprint ui(&browser);
        ui.icon("/home/commander/Downloads/ChatGPT Image May 10, 2025, 02_34_14 PM.png");
        ui.title(tr("Users"));
        ui.column("id").label(tr("ID")).size(100);
        ui.column("name").label(tr("Name")).resize(QHeaderView::Stretch);
        ui.column("score").label(tr("Score")).resize(QHeaderView::ResizeToContents);
        ui.edit<UserWindow>().mainField("id").maxCount(2);
        ui.filter<UserEdit>()->registerFields();
    }

    return app.exec(Widgetry::Application::AutoLauch);
}
