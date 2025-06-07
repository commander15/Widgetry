#ifndef WIDGETRY_DATASELECTOR_H
#define WIDGETRY_DATASELECTOR_H

#include <Widgetry/global.h>

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qheaderview.h>

namespace DataGate {
class DataQuery;
class DataResponse;
class AbstractDataManager;
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

class DataSelectorPrivate;
class WIDGETRY_EXPORT DataSelector : public QDialog
{
    Q_OBJECT

public:
    enum SelectorOption {
        SearchByText,
        InfiniteLoad,
        NoButtons
    };

    DataSelector(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataSelector();

    QString searchText() const;
    Q_SLOT void search(const QString &query);
    Q_SLOT void refresh();
    Q_SLOT void clear();
    void setLabel(int index, const QString &name);
    void setResizeMode(int index, QHeaderView::ResizeMode mode);

    QStringList fields() const;
    void setFields(const QStringList &fields);

    bool hasOption(SelectorOption option) const;
    void setOption(SelectorOption option, bool on = true);

    Jsoner::Object selectedObject() const;
    Jsoner::Array selectedObjects() const;

    DataGate::DataQuery searchQuery() const;
    void setSearchQuery(const DataGate::DataQuery &query);

    DataGate::AbstractDataManager *dataManager() const;
    void setDataManager(DataGate::AbstractDataManager *manager);

private:
    Q_SLOT void fetchCompletions(const QString &query);
    Q_SLOT void sortData(int index, Qt::SortOrder order);
    Q_SLOT void fetchData(const QString &query, bool force = false);
    Q_SLOT void fetchMore();
    void processResponse(const DataGate::DataResponse &response);

    Ui::DataSelector *ui;
    QScopedPointer<DataSelectorPrivate> d_ptr;

    friend class DataSelectorPrivate;
};

} // namespace Widgetry

#endif // WIDGETRY_DATASELECTOR_H
