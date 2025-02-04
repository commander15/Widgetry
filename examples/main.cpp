#include <QApplication>
#include <QDialog>

#include "userinterface.h"

#include <Widgetry/widgetry.h>

void monitor(QDialog *d);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Widgetry::init(app);

    UserInterface ui;
    ui.show();

    return app.exec();
}

void monitor(QDialog *d)
{
    QObject::connect(d, &QDialog::finished, d, [](int r) {
        qDebug() << "Dialog exit code:" << r;
    });
}
