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
        object.insert("id", i + 1);
        object.insert("name", "John Doe " + QString::number(gen->bounded(1500, 2025)));
        object.insert("score", gen->bounded(0, pageCount * itemsPerPage));
        m_objects.append(object);
    }
}

bool DataController::hasFeature(Feature feature, DataGate::AbstractDataClient *client) const
{
    return client;
}

void DataController::fetchSomeSearchSuggestions(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    Jsoner::Array results;
    for (const QJsonValue &value : std::as_const(m_objects)) {
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
    const QString q = query.string();
    const QVariantHash filters = query.filters();

    Jsoner::Array objects;

    if (q.isEmpty() && filters.isEmpty()) {
        objects = m_objects;
    } else {
        for (const QJsonValue &value : std::as_const(m_objects)) {
            const Jsoner::Object object = value.toObject();

            if (!q.isEmpty() && object.string("name").contains(q)) {
                objects.append(object);
                continue;
            }

            QString name = filters.value("name").toString();
            if (!name.isEmpty() && object.string("name").contains(name)) {
                objects.append(object);
                continue;
            }

            int score = filters.value("score", 0).toInt();
            if (score > 0 && object.integer("score") == score) {
                objects.append(object);
                continue;
            }

            if (q.isEmpty() && name.isEmpty() && score == 0)
                objects.append(object);
        }
    }

    int currentPage = query.page();
    int totalPage = pageCount;

    DataResponse response;
    response.setPageCount(pageCount);
    response.setSuccess(true);

    if (totalPage <= 0) {
        response.setPage(1);
        onResponse(response);
        return;
    }

    if (currentPage > totalPage) {
        currentPage = 1;
    }

    const int offset = (currentPage - 1) * itemsPerPage;

    Jsoner::Array result;
    for (int i(offset); i < objects.size(); ++i) {
        result.append(objects.at(i));
        if (result.size() == itemsPerPage)
            break;
    }

    response.setArray(result);
    response.setPage(currentPage);
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

void DataController::deleteOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    const int id = query.object().integer("id");

    DataResponse response;
    if (id <= m_objects.size()) {
        m_objects.removeAt(id - 1);
        response.setObject(query.object());
        response.setSuccess(true);
    }
    onResponse(response);
}

void DataController::deleteManyObjects(const DataQuery &query, const DataQueryProgressCallback &onProgress, const DataQueryResponseCallback &onResponse)
{
    const int id = query.object().integer("id");
    auto it = std::find_if(m_objects.begin(), m_objects.end(), [&id](const QJsonValue &value) {
        return value.toObject().value("id").toInt() == id;
    });

    if (it != m_objects.end()) {
        m_objects.erase(it);
        DataResponse response;
        response.setSuccess(true);
        onResponse(response);
    } else {
        onResponse(DataResponse());
    }
}
