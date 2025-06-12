#ifndef WIDGETRY_DATABROWSERBLUEPRINT_H
#define WIDGETRY_DATABROWSERBLUEPRINT_H

#include <Widgetry/global.h>
#include <Widgetry/databrowser.h>
#include <Widgetry/abstractdataedit.h>
#include <Widgetry/databrowsertablecolumnbuilder.h>
#include <Widgetry/dataeditfactory.h>

#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qtoolbutton.h>

class QSpacerItem;
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

    void parameter(const QString &name, const QVariant &value);

    void topWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    void topLayout(QLayout *layout, int stretch = 0);
    void topSpacer(QSpacerItem *spacer);

    QAbstractButton *button(const QString &name, const QIcon &icon);
    QAbstractButton *button(const QString &name, const QIcon &icon, const QString &text);
    template<typename Functor> QAbstractButton *button(const QString &name, const QIcon &icon, QObject *context, Functor &&functor);
    template<typename Functor> QAbstractButton *button(const QString &name, const QIcon &icon, const QString &text, QObject *context, Functor &&functor);
    void button(QAbstractButton *button);

    template<typename T> T *filter();
    void filter(AbstractDataEdit *filter);

    DataBrowserTableColumnBuilder column(const QString &field);

    template<typename T> T *delegate(QObject *parent = nullptr);
    void delegate(QAbstractItemDelegate *delegate);

    template<typename Functor> QAction *menuAction(const QString &name, const QIcon &icon, QObject *context, Functor &&functor);
    template<typename Functor> QAction *menuAction(const QString &name, const QString &text, QObject *context, Functor &&functor);
    template<typename Functor> QAction *menuAction(const QString &name, const QIcon &icon, const QString &text, QObject *context, Functor &&functor);
    QAction *menuAction(const QString &name, const QIcon &icon, const QString &text);
    void menuAction(QAction *action);

    template<typename Handler> Handler *handler();
    void handler(AbstractDataBrowserHandler *handler);

    template<typename T> DataEditFactory<T> &edit();
    void edit(AbstractDataEditFactory *factory);

    template<typename T> T *data();
    void data(DataGate::AbstractDataManager *manager);

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
inline T *DataBrowserBlueprint::delegate(QObject *parent)
{
    T *delegate = new T(parent ? parent : browser());
    delegate(delegate);
    return delegate;
}

template<typename Functor>
QAction *DataBrowserBlueprint::menuAction(const QString &name, const QIcon &icon, QObject *context, Functor &&functor)
{
    QAction *action = menuAction(name, icon, QString());
    QObject::connect(action, &QAction::triggered, context, functor);
    return action;
}

template<typename Functor>
QAction *DataBrowserBlueprint::menuAction(const QString &name, const QString &text, QObject *context, Functor &&functor)
{
    QAction *action = menuAction(name, QIcon(), text);
    QObject::connect(action, &QAction::triggered, context, functor);
    return action;
}

template<typename Functor>
QAction *DataBrowserBlueprint::menuAction(const QString &name, const QIcon &icon, const QString &text, QObject *context, Functor &&functor)
{
    QAction *action = menuAction(name, icon, text);
    QObject::connect(action, &QAction::triggered, context, functor);
    return action;
}

template<typename Handler>
Handler *DataBrowserBlueprint::handler()
{
    Handler *handler = new Handler();
    this->handler(handler);
    return handler;
}

template<typename T>
DataEditFactory<T> &DataBrowserBlueprint::edit()
{
    DataEditFactory<T> *factory = new DataEditFactory<T>();
    this->edit(factory);
    return *factory;
}

template<typename T>
inline T *DataBrowserBlueprint::data()
{
    T *controller = new T();
    data(controller);
    return controller;
}

} // namespace Widgetry

#endif // WIDGETRY_DATABROWSERBLUEPRINT_H
