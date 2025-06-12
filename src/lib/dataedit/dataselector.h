#ifndef WIDGETRY_DATASELECTOR_H
#define WIDGETRY_DATASELECTOR_H

#include <Widgetry/global.h>

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qheaderview.h>

namespace DataGate {
class DataRequest;
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

class DataBrowser;

class DataSelectorPrivate;
class WIDGETRY_EXPORT DataSelector : public QDialog
{
    Q_OBJECT

public:
    enum SelectorOption {
        SearchByText,
        AlternatingRowColors,
        SingleSelection,
        MultiSelection,
        ExtendedSelection,
        ContiguousSelection,
        InfiniteLoad,
        AcceptOnDoubleClick,
        WithButtons
    };

    DataSelector(DataBrowser *browser, Qt::WindowFlags flags = Qt::WindowFlags());
    DataSelector(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataSelector();

    QString searchText() const;
    Q_SLOT void search(const QString &query);
    Q_SLOT void refresh();
    Q_SLOT void clear();

    void setLabel(const QString &field, const QString &name);
    void setResizeMode(const QString &field, QHeaderView::ResizeMode mode);

    Jsoner::Object selectedObject() const;
    Jsoner::Array selectedObjects() const;

    Jsoner::Array modelObjects() const;
    void setModelObjects(const Jsoner::Array &objects);

    bool hasOption(SelectorOption option) const;
    void setOption(SelectorOption option, bool on = true);

    QStringList fields() const;
    void setFields(const QStringList &fields);

    DataGate::DataRequest request() const;
    void setRequest(const DataGate::DataRequest &request);

    DataGate::AbstractDataManager *manager() const;
    void setManager(DataGate::AbstractDataManager *manager);

protected:
    void showEvent(QShowEvent *event) override;

private:
    Q_SLOT void fetchCompletions(const QString &query);
    Q_SLOT void sortData(int index, Qt::SortOrder order);
    Q_SLOT void fetchData(const QString &query, bool force = false);
    Q_SLOT void fetchMore();

    Q_SLOT void processDoubleClick(const QModelIndex &index);

    void processResponse(const DataGate::DataResponse &response);

    Ui::DataSelector *ui;
    QScopedPointer<DataSelectorPrivate> d_ptr;

    friend class DataSelectorPrivate;
};

} // namespace Widgetry

#endif // WIDGETRY_DATASELECTOR_H
