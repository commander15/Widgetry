#ifndef WIDGETRY_ABSTRACTDATACONTROLLER_H
#define WIDGETRY_ABSTRACTDATACONTROLLER_H

#include <Widgetry/global.h>

#include <Jsoner/object.h>

#include <QtCore/qobject.h>

namespace Widgetry {

class DataInterface;

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

    int page() const;
    void setPage(int page);

    Jsoner::Object object() const;
    void setObject(const Jsoner::Object &object);

    Jsoner::Array objects() const;
    void setObjects(const Jsoner::Array &objects);

    QVariantHash parameters() const;
    void setParameters(const QVariantHash &parameters);

private:
    QSharedDataPointer<DataQueryData> d_ptr;
};

class DataResponseData;
class WIDGETRY_EXPORT DataResponse
{
public:
    DataResponse();
    DataResponse(const DataResponse &other);
    ~DataResponse();

    DataResponse &operator=(const DataResponse &other);

    QString text() const;
    void setText(const QString &text);

    QString informativeText() const;
    void setInformativeText(const QString &text);

    QString detailedText() const;
    void setDetailedText(const QString &text);

    int page() const;
    void setPage(int page);

    int pageCount() const;
    void setPageCount(int count);

    bool hasData(const QString &name) const;
    QVariant data(const QString &name) const;
    void setData(const QString &name, const QVariant &value);
    QStringList dataNames() const;

    bool isSuccess() const;
    void setSuccess(bool success);

private:
    QSharedPointer<DataResponseData> d_ptr;
};

class DataQuery;
class DataResponse;

typedef std::function<void(int, int)> DataQueryProgressCallback;
typedef std::function<void(const DataResponse &response)> DataQueryResponseCallback;

class WIDGETRY_EXPORT AbstractDataController
{
public:
    explicit AbstractDataController() = default;
    virtual ~AbstractDataController() = default;

    void fetchObjects(const DataQuery &query, const DataQueryResponseCallback &onResponse);
    void fetchObjects(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse);

    void fetchObject(const DataQuery &query, int targetRequestType, const DataQueryResponseCallback &onResponse);
    void fetchObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse);

    void addObject(const DataQuery &query, const DataQueryResponseCallback &onResponse);
    void addObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse);

    void editObject(const DataQuery &query, const DataQueryResponseCallback &onResponse);
    void editObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse);

    void deleteObjects(const DataQuery &query, const DataQueryResponseCallback &onResponse);
    void deleteObjects(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse);

    virtual void fetchManyObjects(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse) = 0;
    virtual void fetchOneObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse) = 0;
    virtual void addOneObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse) = 0;
    virtual void editOneObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse) = 0;
    virtual void deleteManyObjects(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse) = 0;
};

} // namespace Widgetry

#endif // WIDGETRY_ABSTRACTDATACONTROLLER_H
