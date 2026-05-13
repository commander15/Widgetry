#include "mainwindow.h"
#include "mainwindow_p.h"

#include <Widgetry/widget.h>

#include <QtCore/qsettings.h>

#include <QtWidgets/qmenu.h>
#include <QtWidgets/qtoolbar.h>
#include <QtWidgets/qstackedwidget.h>
#include <QtWidgets/qactiongroup.h>

namespace Widgetry {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , d_ptr{new MainWindowPrivate(this)}
{
}

MainWindow::~MainWindow()
{
}

QMenu *MainWindow::viewMenu() const
{
    return d_ptr->viewMenu;
}

void MainWindow::setViewMenu(QMenu *menu)
{
    d_ptr->viewMenu = menu;
}

QToolBar *MainWindow::toolBar() const
{
    return d_ptr->toolbar;
}

void MainWindow::setToolBar(QToolBar *bar)
{
    d_ptr->toolbar = bar;
}

QStackedWidget *MainWindow::stackedWidget() const
{
    return d_ptr->stackedWidget;
}

void MainWindow::setStackedWidget(QStackedWidget *widget)
{
    d_ptr->stackedWidget = widget;
}

void MainWindow::addWidget(Widget *widget)
{
    if (d_ptr->stackedWidget)
        d_ptr->stackedWidget->addWidget(widget);
    registerWidget(widget->action(), widget);
}

void MainWindow::registerWidget(Widget *widget)
{
    registerWidget(widget->action(), widget);
}

void MainWindow::registerWidget(QAction *action, Widget *widget)
{
    if (d_ptr->widgets.contains(widget))
        return;

    if (action) {
        widget->setIcon(action->icon());
        widget->setTitle(action->text());
        widget->setAction(action);

        action->setCheckable(true);
        d_ptr->actions->addAction(action);

        if (d_ptr->viewMenu != nullptr)
            d_ptr->viewMenu->addAction(action);

        if (d_ptr->toolbar != nullptr)
            d_ptr->toolbar->addAction(action);

        if (d_ptr->stackedWidget != nullptr) {
            if (d_ptr->stackedWidget->currentWidget() == widget)
                action->setChecked(true);

            connect(action, &QAction::triggered, this, [this, widget] {
                d_ptr->stackedWidget->setCurrentWidget(widget);
            });
        }
    }

    d_ptr->widgets.append(widget);
}

void MainWindow::loadSettings(QSettings *settings)
{
    settings->beginGroup("window");

    if (settings->contains("geo"))
        restoreGeometry(settings->value("geo").toByteArray());

    if (settings->contains("state"))
        restoreState(settings->value("state").toByteArray());

    settings->endGroup();

    for (Widget *widget : std::as_const(d_ptr->widgets)) {
        const QByteArray id = widget->id();
        if (id.isEmpty()) continue;

        settings->beginGroup(id);
        widget->loadSettings(settings);
        settings->endGroup();
    }
}

void MainWindow::saveSettings(QSettings *settings)
{
    settings->beginGroup("window");
    settings->setValue("geo", saveGeometry());
    settings->setValue("state", saveState());
    settings->endGroup();

    for (Widget *widget : std::as_const(d_ptr->widgets)) {
        const QByteArray id = widget->id();
        if (id.isEmpty()) continue;

        settings->beginGroup(id);
        widget->saveSettings(settings);
        settings->endGroup();
    }
}

MainWindowPrivate::MainWindowPrivate(MainWindow *q)
    : q_ptr{q}
    , viewMenu{nullptr}
    , toolbar{nullptr}
    , stackedWidget{nullptr}
    , actions{new QActionGroup(q)}
{
    actions->setExclusive(true);
}

} // namespace Widgetry
