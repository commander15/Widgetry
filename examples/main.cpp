#include <QApplication>
#include <QDialog>

#include "logincontroller.h"
#include "userinterface.h"

#include <Widgetry/widgetry.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Widgetry::init(app);

    DataGate::Authenticator::init<LoginController>();

    Widgetry::LoginDialog login;
    login.setIdentifierType(Widgetry::LoginDialog::LoginIdentifier);
    login.showOnLogOut();
    login.open();

    UserInterface ui;
    QObject::connect(DataGate::Authenticator::global(), &DataGate::Authenticator::loggedIn, &ui, [&ui](const Jsoner::Object &) {
        ui.show();
    });

    return app.exec();
}
