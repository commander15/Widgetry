#ifndef WIDGETRY_DATAINTERFACEBASE_H
#define WIDGETRY_DATAINTERFACEBASE_H

#include <Widgetry/global.h>

#include <QtWidgets/qwidget.h>

class QLineEdit;
class QAbstractButton;
class QTableView;
class QSpinBox;
class QMenu;

namespace Jsoner {
class TableModel;
class Object;
}

namespace Widgetry {

namespace Ui {
class DataInterface;
}

class DataEdit;

class DataInterfaceBasePrivate;
class WIDGETRY_EXPORT DataInterfaceBase
{
public:
    DataInterfaceBase(Ui::DataInterface *ui, DataInterfaceBasePrivate *d);
    virtual ~DataInterfaceBase();

public:
    QWidget *topBar() const;

    QLineEdit *searchBar() const;
    QAbstractButton *filterToggleButton() const;

    QAbstractButton *refreshButton() const;
    QAbstractButton *addButton() const;
    QAbstractButton *editButton() const;
    QAbstractButton *deleteButton() const;

    void addButton(QAbstractButton *button);
    void insertButton(QAbstractButton *before, QAbstractButton *button);
    void removeButton(QAbstractButton *button);

    QWidget *filterWidget() const;
    QAbstractButton *filterButton() const;

    QTableView *tableView() const;
    Jsoner::TableModel *tableModel() const;

    QSpinBox *pageInput() const;
    QAbstractButton *previousPageButton() const;
    QAbstractButton *nextPageButton() const;

    QMenu *contextMenu() const;
    DataEdit *dataEdit() const;

protected:
    Jsoner::Object currentObject() const;
    QList<Jsoner::Object> selectedObjects() const;

    virtual void showObject(const Jsoner::Object &object);
    virtual void addObject(const Jsoner::Object &object);
    virtual void editObject(const Jsoner::Object &object);

    virtual bool canDeleteObjects(const QList<Jsoner::Object> &objects);
    virtual void deleteObjects(const QList<Jsoner::Object> &objects);

    void showContextMenu(const QList<Jsoner::Object> &objects, const QPoint &pos);
    virtual bool prepareContextMenu(const QList<Jsoner::Object> &objects, QMenu *menu);

protected:
    void setFilterWidget(QWidget *widget);
    void setTableModel(Jsoner::TableModel *model);
    void setDataEdit(DataEdit *edit);
    void setContextMenu(QMenu *menu, bool addDefaultActions = true);

    Ui::DataInterface *ui;

private:
    DataInterfaceBasePrivate *d;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACEBASE_H
