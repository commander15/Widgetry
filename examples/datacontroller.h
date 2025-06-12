#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <DataGate/abstractdatamanager.h>

#include <Jsoner/array.h>

class DataController : public DataGate::AbstractDataManager
{
public:
    DataController();

    bool hasFeature(Feature feature, DataGate::AbstractDataClient *client) const override;

    void fetchSomeSearchSuggestions(const DataGate::DataRequest &request, const DataGate::DataRequestCallback &onProgress, const DataGate::DataResponseCallback &onResponse) override;
    void fetchManyObjects(const DataGate::DataRequest &request, const DataGate::DataRequestCallback &onProgress, const DataGate::DataResponseCallback &onResponse) override;
    void fetchOneObject(const DataGate::DataRequest &request, const DataGate::DataRequestCallback &onProgress, const DataGate::DataResponseCallback &onResponse) override;
    void addOneObject(const DataGate::DataRequest &request, const DataGate::DataRequestCallback &onProgress, const DataGate::DataResponseCallback &onResponse) override;
    void editOneObject(const DataGate::DataRequest &request, const DataGate::DataRequestCallback &onProgress, const DataGate::DataResponseCallback &onResponse) override;
    void deleteOneObject(const DataGate::DataRequest &request, const DataGate::DataRequestCallback &onProgress, const DataGate::DataResponseCallback &onResponse) override;
    void deleteManyObjects(const DataGate::DataRequest &request, const DataGate::DataRequestCallback &onProgress, const DataGate::DataResponseCallback &onResponse) override;

private:
    Jsoner::Array m_objects;

    int pageCount = 5;
    int itemsPerPage = 20;
};

#endif // DATACONTROLLER_H
