#ifndef WIDGETRY_DATABROWSERBLUEPRINT_H
#define WIDGETRY_DATABROWSERBLUEPRINT_H

#include <Widgetry/global.h>
#include <Widgetry/databrowser.h>
#include <Widgetry/abstractdataedit.h>
#include <Widgetry/dataeditfactory.h>

#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qtoolbutton.h>

class QAbstractButton;

namespace Widgetry {

class DataBrowserBlueprintPrivate;
class WIDGETRY_EXPORT DataBrowserBlueprint
{
public:
    DataBrowserBlueprint(DataBrowser *browser);
    virtual ~DataBrowserBlueprint();

    QIcon icon(const QString &fileName);
    void icon(const QIcon &icon);

    void title(const QString &title);

    QAction *action(const QString &text);
    QAction *action(const QIcon &icon, const QString &text);
    template<typename Functor>QAction *action(const QString &text, QObject *context, Functor &&functor);
    template<typename Functor>QAction *action(const QIcon &icon, const QString &text, QObject *context, Functor &&functor);
    void action(QAction *action);

    QAbstractButton *button(const QString &name, const QIcon &icon);
    QAbstractButton *button(const QString &name, const QIcon &icon, const QString &text);
    template<typename Functor> QAbstractButton *button(const QString &name, const QIcon &icon, QObject *context, Functor &&functor);
    template<typename Functor> QAbstractButton *button(const QString &name, const QIcon &icon, const QString &text, QObject *context, Functor &&functor);
    void button(QAbstractButton *button);

    template<typename T> T *filter();
    void filter(AbstractDataEdit *filter);

    template<typename T> T *tableDelegate(QObject *parent = nullptr);
    void tableDelegate(QAbstractItemDelegate *delegate);

    int tableColumn(const QString &field, QHeaderView::ResizeMode resizeMode = QHeaderView::Stretch);
    int tableColumn(const QString &label, const QString &field, QHeaderView::ResizeMode resizeMode = QHeaderView::Stretch);
    void tableColumn(int index, const QString &label);

    template<typename Functor> QAction *contextMenuAction(const QIcon &icon, QObject *context, Functor &&functor);
    template<typename Functor> QAction *contextMenuAction(const QString &text, QObject *context, Functor &&functor);
    template<typename Functor> QAction *contextMenuAction(const QIcon &icon, const QString &text, QObject *context, Functor &&functor);
    void contextMenuAction(QAction *action);

    template<typename T> DataEditFactory<T> *edit();
    void edit(AbstractDataEditFactory *factory);

    template<typename T> T *dataManager();
    void dataManager(DataGate::AbstractDataManager *manager);

    void cancel();

    DataBrowser *browser() const;

private:
    QScopedPointer<DataBrowserBlueprintPrivate> d_ptr;

    friend class AbstractDataBrowserBlueprintTask;
};

template<typename Functor>
QAction *DataBrowserBlueprint::action(const QString &text, QObject *context, Functor &&functor)
{
    QAction *action = this->action(QIcon(), text);
    QObject::connect(action, &QAction::triggered, context, functor);
    return action;
}

template<typename Functor>
QAction *DataBrowserBlueprint::action(const QIcon &icon, const QString &text, QObject *context, Functor &&functor)
{
    QAction *action = this->action(icon, text);
    QObject::connect(action, &QAction::triggered, context, functor);
    return action;
}

template<typename Functor>
QAbstractButton *DataBrowserBlueprint::button(const QString &name, const QIcon &icon, QObject *context, Functor &&functor)
{
    auto *btn = button(name, icon);
    QObject::connect(btn, &QAbstractButton::clicked, context, functor);
    return btn;
}

template<typename Functor>
QAbstractButton *DataBrowserBlueprint::button(const QString &name, const QIcon &icon, const QString &text, QObject *context, Functor &&functor)
{
    auto *btn = button(name, icon, text);
    QObject::connect(btn, &QAbstractButton::clicked, context, functor);
    return btn;
}

template<typename T>
T *DataBrowserBlueprint::filter()
{
    T *filter = new T(browser());
    this->filter(filter);
    return filter;
}

template<typename T>
inline T *DataBrowserBlueprint::tableDelegate(QObject *parent)
{
    T *delegate = new T(parent ? parent : browser());
    tableDelegate(delegate);
    return delegate;
}

template<typename Functor>
QAction *DataBrowserBlueprint::contextMenuAction(const QIcon &icon, QObject *context, Functor &&functor)
{
    auto *action = new QAction(icon, QString(), browser());
    QObject::connect(action, &QAction::triggered, context, functor);
    contextMenuAction(action);
    return action;
}

template<typename Functor>
QAction *DataBrowserBlueprint::contextMenuAction(const QString &text, QObject *context, Functor &&functor)
{
    auto *action = new QAction(text, browser());
    QObject::connect(action, &QAction::triggered, context, functor);
    contextMenuAction(action);
    return action;
}

template<typename Functor>
QAction *DataBrowserBlueprint::contextMenuAction(const QIcon &icon, const QString &text, QObject *context, Functor &&functor)
{
    auto *action = new QAction(icon, text, browser());
    QObject::connect(action, &QAction::triggered, context, functor);
    contextMenuAction(action);
    return action;
}

template<typename T>
DataEditFactory<T> *DataBrowserBlueprint::edit()
{
    DataEditFactory<T> *factory = new DataEditFactory<T>();
    this->edit(factory);
    return factory;
}

template<typename T>
inline T *DataBrowserBlueprint::dataManager()
{
    T *controller = new T();
    dataManager(controller);
    return controller;
}

} // namespace Widgetry

#endif // WIDGETRY_DATABROWSERBLUEPRINT_H
