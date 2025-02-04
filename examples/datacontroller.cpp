#include "datacontroller.h"

#include <Widgetry/dataresponse.h>

DataController::DataController()
{
}

void DataController::fetchManyObjects(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    onResponse(Widgetry::DataResponse());
}

void DataController::fetchOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    onResponse(Widgetry::DataResponse());
}

void DataController::addOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    onResponse(Widgetry::DataResponse());
}

void DataController::editOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    onResponse(Widgetry::DataResponse());
}

void DataController::deleteManyObjects(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    onResponse(Widgetry::DataResponse());
}
