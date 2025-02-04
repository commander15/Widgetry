#include "datacontroller.h"

#include <Widgetry/dataresponse.h>

#include <Jsoner/array.h>
#include <Jsoner/object.h>

DataController::DataController()
{
}

void DataController::fetchManyObjects(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    Jsoner::Array objects;

    Jsoner::Object object;
    object.insert("name", "John Doe");

    int count = 14;
    for (int i(0); i < count; ++i) {
        object.insert("id", i + 1);
        object.insert("score", rand());
        objects.append(object);
    }

    Widgetry::DataResponse response;
    response.setObjects(objects);
    response.setPage(2);
    response.setPageCount(5);
    response.setSuccess(true);
    onResponse(response);
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
