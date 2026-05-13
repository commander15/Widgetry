#ifndef WIDGETRY_MAINWINDOW_H
#define WIDGETRY_MAINWINDOW_H

#include <Widgetry/global.h>

#include <QtWidgets/qmainwindow.h>

class QMenu;
class QToolBar;
class QStackedWidget;
class QSettings;

namespace Widgetry {

class Widget;

class MainWindowPrivate;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    QMenu *viewMenu() const;
    void setViewMenu(QMenu *menu);

    QToolBar *toolBar() const;
    void setToolBar(QToolBar *bar);

    QStackedWidget *stackedWidget() const;
    void setStackedWidget(QStackedWidget *widget);

    void addWidget(Widget *widget);
    void registerWidget(Widget *widget);
    void registerWidget(QAction *action, Widget *widget);

    void loadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);

private:
    QScopedPointer<MainWindowPrivate> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_MAINWINDOW_H

