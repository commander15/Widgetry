#ifndef WIDGETRY_DATAINTERFACEBLUEPRINT_H
#define WIDGETRY_DATAINTERFACEBLUEPRINT_H

#include <Widgetry/global.h>
#include <Widgetry/datainterface.h>

#include <QtWidgets/qheaderview.h>

#include <QtCore/qscopedpointer.h>

namespace Jsoner {
class TableModel;
}

namespace Widgetry {

class DataInterface;
class AbstractDataEdit;
class AbstractDataController;

class DataInterfaceBlueprintPrivate;
class WIDGETRY_EXPORT DataInterfaceBlueprint
{
public:
    DataInterfaceBlueprint(DataInterface *interface);
    ~DataInterfaceBlueprint();

    template<typename T> T *filter();
    void filter(QWidget *filter);

    int tableHeader(const QString &label);
    int tableHeader(const QString &label, const QString &field, QHeaderView::ResizeMode resizeMode);
    int tableHeader(const QString &field, QHeaderView::ResizeMode resizeMode);

    template<typename T> T *tableDelegate(QObject *parent = nullptr);
    void tableDelegate(QAbstractItemDelegate *delegate);

    template<typename T> T *tableModel(QObject *parent = nullptr);
    void tableModel(Jsoner::TableModel *model);

    QMenu *contextMenu(bool addDefaultActions = true);
    void contextMenu(QMenu *menu, bool addDefaultActions = true);

    QAction *contextMenuAction(const QIcon &icon);
    QAction *contextMenuAction(const QString &text);
    QAction *contextMenuAction(const QIcon &icon, const QString &text);
    void contextMenuAction(QAction *action);

    QAction *contextMenuSeparator();

    template<typename T> T *edit();
    template<typename T> T *edit(QWidget *parent);
    template<typename T> T *edit(bool putOnDialog, QWidget *parent = nullptr);
    void edit(AbstractDataEdit *edit);
    void edit(AbstractDataEdit *edit, bool putOnDialog);

    template<typename T> T *dataController();
    void dataController(AbstractDataController *controller);

    DataInterface *interface() const;

private:
    bool isInit() const;

    QScopedPointer<DataInterfaceBlueprintPrivate> d_ptr;
};

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

template<typename T>
T *DataInterfaceBlueprint::edit()
{
    T *edit = new T(interface());
    this->edit(edit);
    return edit;
}

template<typename T>
T *DataInterfaceBlueprint::edit(QWidget *parent)
{
    T *edit = new T(parent);
    this->edit(edit);
    return edit;
}

template<typename T>
inline T *DataInterfaceBlueprint::edit(bool putOnDialog, QWidget *parent)
{
    T *edit = new T(parent ? parent : (putOnDialog ? nullptr : interface()));
    this->edit(edit, putOnDialog);
    return edit;
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
