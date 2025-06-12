#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->usersPage->setAction(ui->actionUsers);
}

MainWindow::~MainWindow()
{
    delete ui;
}
