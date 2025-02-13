#ifndef WIDGETRY_DATASELECTORWIDGET_H
#define WIDGETRY_DATASELECTORWIDGET_H

#include <Widgetry/global.h>

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qheaderview.h>

namespace Jsoner {
class Object;
class Array;
}

class QTableView;

namespace Widgetry {

namespace Ui {
class DataSelectorWidget;
}

class DataQuery;
class AbstractDataController;

class DataSelectorWidgetPrivate;
class WIDGETRY_EXPORT DataSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    DataSelectorWidget(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~DataSelectorWidget();

    QString searchText() const;

    DataQuery searchQuery() const;
    void setSearchQuery(const DataQuery &query);

    void setHeader(int index, const QString &name);
    void setResizeMode(int index, QHeaderView::ResizeMode mode);

    QStringList fields() const;
    void setFields(const QStringList &fields);

    Jsoner::Object selectedObject() const;
    Jsoner::Array selectedObjects() const;

    QTableView *tableView() const;

    AbstractDataController *dataController() const;
    void setDataController(AbstractDataController *controller);

public slots:
    void search(const QString &query);
    void search(const DataQuery &query);
    void refresh();
    void clear();

private slots:
    void fetchCompletions(const QString &query);
    void autoSearch(const QString &query);

private:
    Ui::DataSelectorWidget *ui;
    QScopedPointer<DataSelectorWidgetPrivate> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DATASELECTORWIDGET_H
