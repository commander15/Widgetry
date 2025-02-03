#ifndef WIDGETRY_ABSTRACTDATACONTROLLER_H
#define WIDGETRY_ABSTRACTDATACONTROLLER_H

#include <Widgetry/global.h>

#include <functional>

namespace Widgetry {

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
