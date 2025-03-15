#ifndef WIDGETRY_DATASELECTOR_H
#define WIDGETRY_DATASELECTOR_H

#include <Widgetry/global.h>

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qheaderview.h>

namespace DataGate {
class DataQuery;
class AbstractDataController;
}

namespace Jsoner {
class Object;
class Array;
}

class QTableView;

namespace Widgetry {

namespace Ui {
class DataSelector;
}

class DataSelectorWidget;

class WIDGETRY_EXPORT DataSelector : public QDialog
{
    Q_OBJECT

public:
    DataSelector(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~DataSelector();

    QString searchText() const;

    DataGate::DataQuery searchQuery() const;
    void setSearchQuery(const DataGate::DataQuery &query);

    void setHeader(int index, const QString &name);
    void setResizeMode(int index, QHeaderView::ResizeMode mode);

    QStringList fields() const;
    void setFields(const QStringList &fields);

    Jsoner::Object selectedObject() const;
    Jsoner::Array selectedObjects() const;

    QTableView *tableView() const;

    DataGate::AbstractDataController *dataController() const;
    void setDataController(DataGate::AbstractDataController *controller);

    DataSelectorWidget *selectorWidget() const;

public slots:
    void search(const QString &query);
    void search(const DataGate::DataQuery &query);
    void refresh();
    void clear();

private slots:
    void closeDialog();

private:
    Ui::DataSelector *ui;
};

} // namespace Widgetry

#endif // WIDGETRY_DATASELECTOR_H
