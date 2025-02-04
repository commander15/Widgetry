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
class Array;
}

namespace Widgetry {

namespace Ui {
class DataInterface;
}

class AbstractDataEdit;

class DataInterfaceForgePrivate;
class WIDGETRY_EXPORT DataInterfaceForge
{
public:
    DataInterfaceForge(DataInterfaceForgePrivate *d);
    ~DataInterfaceForge();

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

    AbstractDataEdit *filterWidget() const;
    QAbstractButton *filterButton() const;

    QTableView *tableView() const;
    Jsoner::TableModel *tableModel() const;

    QSpinBox *pageInput() const;
    QAbstractButton *previousPageButton() const;
    QAbstractButton *nextPageButton() const;

    QMenu *contextMenu() const;
    AbstractDataEdit *dataEdit() const;

public:
    void setFilterWidget(AbstractDataEdit *widget);
    void setTableModel(Jsoner::TableModel *model);
    void setDataEdit(AbstractDataEdit *edit);
    void setDataEdit(AbstractDataEdit *edit, QWidget *dialogParent);
    void setContextMenu(QMenu *menu, bool addDefaultActions = true);

protected:
    bool prepareContextMenu(const Jsoner::Array &objects, QMenu *menu);

private:
    void init();

    bool isBluePrinted() const;
    void setBluePrinted(bool printed = true);

    Ui::DataInterface *ui;
    DataInterfaceForgePrivate *d_ptr;

    friend class DataInterface;
    friend class DataInterfaceBlueprint;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACEBASE_H
