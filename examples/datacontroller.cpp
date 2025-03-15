#include "datacontroller.h"

#include <DataGate/dataquery.h>
#include <DataGate/dataresponse.h>

#include <Jsoner/object.h>

#include <QtCore/qrandom.h>

using namespace DataGate;

DataController::DataController()
{
    Jsoner::Array objects;
    QRandomGenerator *gen = QRandomGenerator::global();

    for (int i(0); i < pageCount * itemsPerPage; ++i) {
        Jsoner::Object object;
        object.insert("id", (i * itemsPerPage) + 1);
        object.insert("name", "John Doe " + QString::number(gen->bounded(1500, 2025)));
        object.insert("score", gen->bounded(0, pageCount * itemsPerPage));
        m_objects.append(object);
    }
}

void DataController::fetchSomeSearchSuggestions(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    Jsoner::Array results;
    for (const QJsonValue &value : m_objects) {
        const QString key = value.toObject().value("name").toString();
        if (key.startsWith(query.string(), Qt::CaseInsensitive))
            results.append(key);
    }

    DataResponse response;
    response.setArray(results);
    response.setSuccess(!results.isEmpty());

    onResponse(response);
}

void DataController::fetchManyObjects(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    Jsoner::Array objects;
    for (const QJsonValue &value : m_objects) {
        const Jsoner::Object object = value.toObject();

        const QString q = query.string();
        if (!q.isEmpty() && !object.string("name").contains(q)) {
            objects.append(object);
            continue;
        }

        const QVariantHash filters = query.filters();
        if (!filters.isEmpty()) {
            bool byPass = false;

            const QStringList names = filters.keys();
            for (const QString &name : names) {
                const QString value = filters.value(name).toString();
                if (object.string(name).contains(value)) {
                    objects.append(object);
                    byPass = true;
                    continue;
                }
            }

            if (byPass)
                continue;
        }
    }

    const int offset = query.page() * itemsPerPage;
    const int pageCount = qCeil<double>(objects.size() / itemsPerPage);
    while (objects.size() > offset)
        objects.removeLast();

    const QString sortField = query.sortField();
    if (!sortField.isEmpty()) {
    }

    DataResponse response;
    if (query.page() > 0 && query.page() <= m_objects.size()) {
        response.setArray(objects);
        response.setPage(query.page());
        response.setPageCount(pageCount);
        response.setSuccess(true);
    }
    onResponse(response);
}

void DataController::fetchOneObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    const int id = query.object().integer("id");

    DataResponse response;
    if (id <= m_objects.size()) {
        response.setObject(m_objects.at(id - 1));
        response.setSuccess(true);
    }
    onResponse(response);
}

void DataController::addOneObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    Jsoner::Object object = query.object();
    object.put("id", m_objects.size());
    m_objects.append(object);

    DataResponse response;
    response.setObject(object);
    response.setSuccess(true);
    onResponse(response);
}

void DataController::editOneObject(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    const int id = query.object().integer("id");

    DataResponse response;
    if (id <= m_objects.size()) {
        m_objects.replace(id - 1, query.object());
        response.setObject(query.object());
        response.setSuccess(true);
    }
    onResponse(response);
}

void DataController::deleteManyObjects(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    onResponse(DataResponse());
}
