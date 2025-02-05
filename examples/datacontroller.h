#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <Widgetry/abstractdatacontroller.h>

#include <Jsoner/array.h>

class DataController : public Widgetry::AbstractDataController
{
public:
    DataController();

    void fetchManyObjects(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse) override;
    void fetchOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse) override;
    void addOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse) override;
    void editOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse) override;
    void deleteManyObjects(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse) override;

private:
    QList<Jsoner::Array> m_objects;

    int pageCount = 5;
    int itemsPerPage = 20;
};

#endif // DATACONTROLLER_H
