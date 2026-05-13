#ifndef WIDGETRY_MAINWINDOW_P_H
#define WIDGETRY_MAINWINDOW_P_H

#include "mainwindow.h"

namespace Widgetry {

class MainWindowPrivate
{
public:
    MainWindowPrivate(MainWindow *q);

    MainWindow *q_ptr;

    QMenu *viewMenu;
    QToolBar *toolbar;
    QStackedWidget *stackedWidget;

    QActionGroup *actions;

    QList<Widget *> widgets;
};

}

#endif // WIDGETRY_MAINWINDOW_P_H
