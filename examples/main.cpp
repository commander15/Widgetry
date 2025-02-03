#include <QApplication>
#include <QDialog>

#include "userinterface.h"
#include "useredit.h"

void monitor(QDialog *d);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    UserInterface ui;
    ui.show();

    UserEdit *e1 = new UserEdit;
    e1->setObject(Jsoner::Object(), Widgetry::DataEdit::AddOperation);
    QDialog *d1 = Widgetry::DataEdit::dialogFromEdit(e1);
    monitor(d1);

    UserEdit *e2 = new UserEdit;
    e2->setObject(Jsoner::Object(), Widgetry::DataEdit::AddOperation);
    e2->registerFields();
    QDialog *d2 = Widgetry::DataEdit::dialogFromEdit(e2);
    monitor(d2);

    d1->show();
    d2->show();

    int e = app.exec();

    delete d1;
    delete d2;
}

void monitor(QDialog *d)
{
    QObject::connect(d, &QDialog::finished, d, [](int r) {
        qDebug() << "Dialog exit code:" << r;
    });
}
