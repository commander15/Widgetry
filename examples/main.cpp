#include <QApplication>
#include <QDialog>

#include "logincontroller.h"
#include "mainwindow.h"

#include <Widgetry/widgetry.h>

int main(int argc, char *argv[])
{
    Widgetry::Application app(argc, argv);

    DataGate::Authenticator::init<LoginController>();

    Widgetry::LoginDialog login;
    login.setIdentifierType(Widgetry::LoginDialog::UsernameIdentifier);
    login.showOnLogOut();
    login.open();

    MainWindow ui;
    QObject::connect(DataGate::Authenticator::global(), &DataGate::Authenticator::loggedIn, &ui, [&ui](const Jsoner::Object &) {
        ui.show();
    });

    return app.exec();
}
