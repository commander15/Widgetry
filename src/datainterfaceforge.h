#ifndef WIDGETRY_DATAINTERFACEBASE_H
#define WIDGETRY_DATAINTERFACEBASE_H

#include <Widgetry/global.h>

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qheaderview.h>

class QLineEdit;
class QAbstractButton;
class QTableView;
class QSpinBox;
class QMenu;

namespace Jsoner {
class TableModel;
class Object;
class Array;
}

namespace Widgetry {

namespace Ui {
class DataInterface;
}

class DataEdit;

class DataInterfaceForgePrivate;
class WIDGETRY_EXPORT DataInterfaceForge
{
public:
    DataInterfaceForge(DataInterfaceForgePrivate *d);
    ~DataInterfaceForge();

    void tableHeader(int index, const QString &label);
    void tableHeader(int index, const QString &label, QHeaderView::ResizeMode resizeMode);
    void tableHeader(int index, const QString &label, const QString &field, QHeaderView::ResizeMode resizeMode);

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

public:
    void setFilterWidget(QWidget *widget);
    void setTableModel(Jsoner::TableModel *model);
    void setDataEdit(DataEdit *edit);
    void setDataEdit(DataEdit *edit, QWidget *dialogParent);
    void setContextMenu(QMenu *menu, bool addDefaultActions = true);

protected:
    bool prepareContextMenu(const Jsoner::Array &objects, QMenu *menu);

private:
    void init();

    Ui::DataInterface *ui;
    DataInterfaceForgePrivate *d_ptr;

    friend class DataInterface;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACEBASE_H
