#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <Widgetry/abstractdatacontroller.h>

class DataController : public Widgetry::AbstractDataController
{
public:
    DataController();

    void fetchManyObjects(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse) override;
    void fetchOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse) override;
    void addOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse) override;
    void editOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse) override;
    void deleteManyObjects(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse) override;
};

#endif // DATACONTROLLER_H
