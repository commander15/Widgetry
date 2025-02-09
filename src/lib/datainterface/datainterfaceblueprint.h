#ifndef WIDGETRY_DATAINTERFACEBLUEPRINT_H
#define WIDGETRY_DATAINTERFACEBLUEPRINT_H

#include <Widgetry/global.h>
#include <Widgetry/datainterface.h>
#include <Widgetry/abstractdataedit.h>
#include <Widgetry/dataeditfactory.h>

#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qtoolbutton.h>

#include <QtCore/qscopedpointer.h>

namespace Widgetry {

class DataInterface;
class AbstractDataEdit;
class AbstractDataEditFactory;
class DataTableModel;
class AbstractDataController;

class DataInterfaceBlueprintPrivate;
class WIDGETRY_EXPORT DataInterfaceBlueprint
{
public:
    DataInterfaceBlueprint(DataInterface *interface);
    ~DataInterfaceBlueprint();

    QIcon icon(const QString &fileName);
    void icon(const QIcon &icon);

    void title(const QString &title);

    QAction *action(const QIcon &icon);
    QAction *action(const QString &text);
    QAction *action(const QIcon &icon, const QString &text);
    void action(QAction *action);

    void search(bool allowed = true);

    template<typename Functor> QAbstractButton *button(const QIcon &icon, QObject *context, Functor &&functor);
    template<typename Functor> QAbstractButton *button(const QString &text, QObject *context, Functor &&functor);
    template<typename Functor> QAbstractButton *button(const QIcon &icon, const QString &text, QObject *context, Functor &&functor);
    void button(QAbstractButton *button);

    template<typename T> T *filter();
    void filter(AbstractDataEdit *filter);

    int tableHeader(const QString &label);
    int tableHeader(const QString &label, const QString &field, QHeaderView::ResizeMode resizeMode);
    int tableHeader(const QString &field, QHeaderView::ResizeMode resizeMode);

    template<typename T> T *tableDelegate(QObject *parent = nullptr);
    void tableDelegate(QAbstractItemDelegate *delegate);

    template<typename T> T *tableModel(QObject *parent = nullptr);
    void tableModel(DataTableModel *model);

    QMenu *contextMenu(bool addDefaultActions = true);
    void contextMenu(QMenu *menu, bool addDefaultActions = true);

    template<typename Functor> QAction *contextMenuAction(const QIcon &icon, QObject *context, Functor &&functor);
    template<typename Functor> QAction *contextMenuAction(const QString &text, QObject *context, Functor &&functor);
    template<typename Functor> QAction *contextMenuAction(const QIcon &icon, const QString &text, QObject *context, Functor &&functor);
    void contextMenuAction(QAction *action);

    QAction *contextMenuSeparator();

    template<typename T> DataEditFactory<T> *edit();
    void edit(AbstractDataEditFactory *factory);

    template<typename T> T *dataController();
    void dataController(AbstractDataController *controller);

    DataInterface *interface() const;

private:
    bool isInit() const;

    QScopedPointer<DataInterfaceBlueprintPrivate> d_ptr;
};

template<typename Functor>
QAbstractButton *DataInterfaceBlueprint::button(const QIcon &icon, QObject *context, Functor &&functor)
{
    auto *btn = new QToolButton(interface());
    btn->setIcon(icon);
    QObject::connect(btn, &QAbstractButton::clicked, context, functor);
    button(btn);
    return btn;
}

template<typename Functor>
QAbstractButton *DataInterfaceBlueprint::button(const QString &text, QObject *context, Functor &&functor)
{
    auto *btn = new QToolButton(interface());
    btn->setText(text);
    QObject::connect(btn, &QAbstractButton::clicked, context, functor);
    button(btn);
    return btn;
}

template<typename Functor>
QAbstractButton *DataInterfaceBlueprint::button(const QIcon &icon, const QString &text, QObject *context, Functor &&functor)
{
    auto *btn = new QToolButton(interface());
    btn->setIcon(icon);
    btn->setText(text);
    QObject::connect(btn, &QAbstractButton::clicked, context, functor);
    button(btn);
    return btn;
}

template<typename T>
T *DataInterfaceBlueprint::filter()
{
    T *filter = new T(interface());
    this->filter(filter);
    return filter;
}

template<typename T>
inline T *DataInterfaceBlueprint::tableDelegate(QObject *parent)
{
    T *delegate = new T(parent ? parent : interface());
    tableDelegate(delegate);
    return delegate;
}

template<typename T>
inline T *DataInterfaceBlueprint::tableModel(QObject *parent)
{
    T *model = new T(parent ? parent : interface());
    tableModel(model);
    return model;
}

template<typename Functor>
QAction *DataInterfaceBlueprint::contextMenuAction(const QIcon &icon, QObject *context, Functor &&functor)
{
    auto *action = new QAction(icon, QString(), interface());
    QObject::connect(action, &QAction::triggered, context, functor);
    contextMenuAction(action);
    return action;
}

template<typename Functor>
QAction *DataInterfaceBlueprint::contextMenuAction(const QString &text, QObject *context, Functor &&functor)
{
    auto *action = new QAction(text, interface());
    QObject::connect(action, &QAction::triggered, context, functor);
    contextMenuAction(action);
    return action;
}

template<typename Functor>
QAction *DataInterfaceBlueprint::contextMenuAction(const QIcon &icon, const QString &text, QObject *context, Functor &&functor)
{
    auto *action = new QAction(icon, text, interface());
    QObject::connect(action, &QAction::triggered, context, functor);
    contextMenuAction(action);
    return action;
}

template<typename T>
DataEditFactory<T> *DataInterfaceBlueprint::edit()
{
    DataEditFactory<T> *factory = new DataEditFactory<T>();
    this->edit(factory);
    return factory;
}

template<typename T>
inline T *DataInterfaceBlueprint::dataController()
{
    T *controller = new T();
    dataController(controller);
    return controller;
}

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACEBLUEPRINT_H
