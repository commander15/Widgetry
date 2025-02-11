#include "abstractdatacontroller.h"

#include <Widgetry/dataresponse.h>

namespace Widgetry {

void AbstractDataController::fetchSuggestions(const DataQuery &query, const DataQueryResponseCallback &onResponse)
{
    fetchSuggestions(query, [](int, int) {}, onResponse);  // Pass a no-op progress callback
}

void AbstractDataController::fetchSuggestions(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    if (onResponse)
    {
        fetchSomeSuggestions(query, onProgress, onResponse);  // Call the virtual method with the given callbacks
    }
}

void AbstractDataController::fetchObjects(const DataQuery &query, const DataQueryResponseCallback &onResponse)
{
    fetchObjects(query, [](int, int) {}, onResponse);  // Pass a no-op progress callback
}

void AbstractDataController::fetchObjects(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    if (onResponse)
    {
        fetchManyObjects(query, onProgress, onResponse);  // Call the virtual method with the given callbacks
    }
}

void AbstractDataController::fetchObject(const DataQuery &query, int targetRequestType, const DataQueryResponseCallback &onResponse)
{
    fetchObject(query, [](int, int) {}, onResponse);  // Pass a no-op progress callback
}

void AbstractDataController::fetchObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    if (onResponse)
    {
        fetchOneObject(query, onProgress, onResponse);  // Call the virtual method with the given callbacks
    }
}

void AbstractDataController::addObject(const DataQuery &query, const DataQueryResponseCallback &onResponse)
{
    addObject(query, [](int, int) {}, onResponse);  // Pass a no-op progress callback
}

void AbstractDataController::addObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    if (onResponse)
    {
        addOneObject(query, onProgress, onResponse);  // Call the virtual method with the given callbacks
    }
}

void AbstractDataController::editObject(const DataQuery &query, const DataQueryResponseCallback &onResponse)
{
    editObject(query, [](int, int) {}, onResponse);  // Pass a no-op progress callback
}

void AbstractDataController::editObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    if (onResponse)
    {
        editOneObject(query, onProgress, onResponse);  // Call the virtual method with the given callbacks
    }
}

void AbstractDataController::deleteObjects(const DataQuery &query, const DataQueryResponseCallback &onResponse)
{
    deleteObjects(query, [](int, int) {}, onResponse);  // Pass a no-op progress callback
}

void AbstractDataController::deleteObjects(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    if (onResponse)
    {
        deleteManyObjects(query, onProgress, onResponse);  // Call the virtual method with the given callbacks
    }
}

void AbstractDataController::fetchSomeSuggestions(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    onResponse(DataResponse());
}

} // namespace Widgetry
