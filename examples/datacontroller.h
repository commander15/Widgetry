#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <DataGate/abstractdatamanager.h>

#include <Jsoner/array.h>

class DataController : public DataGate::AbstractDataManager
{
public:
    DataController();

    bool hasFeature(Feature feature, DataGate::AbstractDataClient *client) const override;

    void fetchSomeSearchSuggestions(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void fetchManyObjects(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void fetchOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void addOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void editOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void deleteOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void deleteManyObjects(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;

private:
    Jsoner::Array m_objects;

    int pageCount = 5;
    int itemsPerPage = 20;
};

#endif // DATACONTROLLER_H
