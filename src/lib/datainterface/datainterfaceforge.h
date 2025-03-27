#ifndef WIDGETRY_DATAINTERFACEBASE_H
#define WIDGETRY_DATAINTERFACEBASE_H

#include <Widgetry/global.h>

#include <QtWidgets/qwidget.h>

#include <QtCore/qobject.h>
#include <abstractdatacontroller.h>

class QLineEdit;
class QCompleter;
class QAbstractButton;
class QTableView;
class QSpinBox;
class QMenu;

namespace DataGate {
class TableModel;
}

namespace Jsoner {
class Object;
class Array;
}

namespace Widgetry {

namespace Ui {
class DataInterface;
}

class AbstractDataEdit;
class AbstractDataEditFactory;

class DataInterfaceForgePrivate;
class WIDGETRY_EXPORT DataInterfaceForge : public QObject
{
    Q_OBJECT

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
    DataGate::TableModel *tableModel() const;

    QSpinBox *pageInput() const;
    QAbstractButton *previousPageButton() const;
    QAbstractButton *nextPageButton() const;

    QMenu *contextMenu() const;
    AbstractDataEditFactory *dataEditFactory() const;

    bool isBluePrinted() const;

public:
    void setFilterWidget(AbstractDataEdit *widget);
    void setDataEdit(AbstractDataEditFactory *factory);
    void setContextMenu(QMenu *menu, bool addDefaultActions = true);

    void setDataController(DataGate::AbstractDataController *controller);

signals:
    void operationSupportChanged(const QString &name);

protected:
    bool prepareContextMenu(const Jsoner::Array &objects, QMenu *menu);

private slots:
    void fetchSearchSuggestions(const QString &query);
    void processFetchedData(const DataGate::DataResponse &response);

    void processOperationSupportChange(const QString &name, bool apply = true);

private:
    void init();
    void initSearchCompletion();
    void initFiltering();
    void initButtons();
    void initTable();
    void initPagination();

    bool applyOperationSupport();

    void setBluePrinted(bool printed = true);

    Ui::DataInterface *ui;
    DataInterfaceForgePrivate *d_ptr;

    friend class DataInterface;
    friend class DataInterfaceBlueprint;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACEBASE_H
