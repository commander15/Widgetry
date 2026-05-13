#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(QWidget *parent)
    : Widgetry::DataWindow("users", parent)
    , ui(new Ui::UserWindow)
{
    ui->setupUi(this);

    registerEdit(ui->edit);

    connect(ui->saveButton, &QAbstractButton::clicked, this, [this] { emit editingFinished(); });
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::render(const Jsoner::Object &object)
{
    setWindowTitle(object.string("name"));
    Widgetry::DataWindow::render(object);
    ui->lcdNumber->display(object.integer("score"));
}

void UserWindow::makeWriteable(bool writeable)
{
    Widgetry::DataWindow::makeWriteable(writeable);
    ui->saveButton->setEnabled(writeable);
}
