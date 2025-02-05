#include "datacontroller.h"

#include <Widgetry/dataquery.h>
#include <Widgetry/dataresponse.h>

#include <Jsoner/object.h>

#include <QtCore/qrandom.h>

DataController::DataController()
{
    QRandomGenerator *gen = QRandomGenerator::global();
    for (int i(0); i < pageCount; ++i) {
        Jsoner::Array objects;

        for (int j(0); j < itemsPerPage; ++j) {
            Jsoner::Object object;
            object.insert("id", (i * itemsPerPage) + j + 1);
            object.insert("name", "John Doe " + QString::number(gen->bounded(1500, 2025)));
            object.insert("score", gen->bounded(0, pageCount * itemsPerPage));
            objects.append(object);
        }

        m_objects.append(objects);
    }
}

void DataController::fetchManyObjects(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    Widgetry::DataResponse response;
    if (query.page() > 0 && query.page() <= m_objects.size()) {
        response.setObjects(m_objects.at(query.page() - 1));
        response.setPage(query.page());
        response.setPageCount(m_objects.size());
        response.setSuccess(true);
    }
    onResponse(response);
}

void DataController::fetchOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    const int id = query.object().integer("id");

    Widgetry::DataResponse response;
    if (id <= m_objects.at(0).size()) {
        response.setObject(m_objects.at(0).at(id - 1));
        response.setSuccess(true);
    }
    onResponse(response);
}

void DataController::addOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    onResponse(Widgetry::DataResponse());
}

void DataController::editOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    const int id = query.object().integer("id");

    Widgetry::DataResponse response;
    if (id <= m_objects.size()) {
        m_objects[0].replace(id - 1, query.object());
        response.setObject(query.object());
        response.setSuccess(true);
    }
    onResponse(response);
}

void DataController::deleteManyObjects(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    onResponse(Widgetry::DataResponse());
}
