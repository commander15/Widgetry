#ifndef WIDGETRY_DATASELECTOR_H
#define WIDGETRY_DATASELECTOR_H

#include <Widgetry/global.h>

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qheaderview.h>

namespace Jsoner {
class Object;
class Array;
}

class QTableView;

namespace Widgetry {

namespace Ui {
class DataSelector;
}

class AbstractDataController;
class DataQuery;

class DataSelectorPrivate;
class WIDGETRY_EXPORT DataSelector : public QDialog
{
    Q_OBJECT

public:
    DataSelector(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataSelector();

    QString searchText() const;

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

private slots:
    void fetchCompletions(const QString &query);
    void autoSearch(const QString &query);
    void closeDialog();

private:
    Ui::DataSelector *ui;
    QScopedPointer<DataSelectorPrivate> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DATASELECTOR_H
