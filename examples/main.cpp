#include <QApplication>
#include <QDialog>

#include "logincontroller.h"
#include "datacontroller.h"
#include "mainwindow.h"

#include <Widgetry/loadindicator.h>

#include <Widgetry/widgetry.h>

int main(int argc, char *argv[])
{
    Widgetry::Application app(argc, argv);

    Widgetry::LoadIndicator indic0, indic1, indic2;
    indic0.show();
    indic1.show();
    indic2.show();

    return app.exec(Widgetry::Application::ManualLaunch);

    LoginController logins;
    app.setLoginManager(&logins);

    DataController data;
    app.setDataManager(&data);

    Widgetry::LoginDialog login;
    login.setIdentifierType(Widgetry::LoginDialog::UsernameIdentifier);
    login.showOnLogOut();
    app.setLoginDialog(&login);

    MainWindow ui;
    app.setMainWidget(&ui);

    return app.exec(Widgetry::Application::AutoLauch);
}
