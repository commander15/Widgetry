#ifndef WIDGETRY_DATAQUERY_H
#define WIDGETRY_DATAQUERY_H

#include <Widgetry/global.h>

#include <QtCore/qshareddata.h>

namespace Jsoner {
class Object;
class Array;
}

namespace Widgetry {

class DataQueryData;
class WIDGETRY_EXPORT DataQuery {
public:
    DataQuery();
    DataQuery(const Jsoner::Object &object);
    DataQuery(const Jsoner::Array &objects);
    DataQuery(const DataQuery &other);
    ~DataQuery();

    DataQuery &operator=(const DataQuery &other);

    QString query() const;
    void setQuery(const QString &query);

    QVariantHash filters() const;
    void setFilters(const QVariantHash &filters);

    QString sortField() const;
    Qt::SortOrder sortOrder() const;
    void setSort(const QString &field, Qt::SortOrder order);

    int page() const;
    void setPage(int page);

    Jsoner::Object object() const;
    void setObject(const Jsoner::Object &object);

    Jsoner::Array objects() const;
    void setObjects(const Jsoner::Array &objects);

    QVariant parameter(const QString &name) const;
    QVariantHash parameters() const;
    void setParameters(const QVariantHash &parameters);

private:
    QSharedDataPointer<DataQueryData> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAQUERY_H
