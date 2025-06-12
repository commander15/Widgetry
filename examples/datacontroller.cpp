#include "datacontroller.h"

#include <DataGate/datarequest.h>
#include <DataGate/dataresponse.h>

#include <Jsoner/object.h>

#include <QtCore/qrandom.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qtimer.h>

#define NETWORK_LATENCY 1000

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
    switch (feature) {
    //case DataGate::AbstractDataManager::SearchByKeywords:
    //case DataGate::AbstractDataManager::SearchByFilters:
        return false;
    }
    return client;
}

void DataController::fetchSomeSearchSuggestions(const DataRequest &request, const DataRequestCallback &onProgress, const DataResponseCallback &onResponse)
{
    Jsoner::Array results;
    for (const QJsonValue &value : std::as_const(m_objects)) {
        const QString key = value.toObject().value("name").toString();
        if (key.startsWith(request.query(), Qt::CaseInsensitive))
            results.append(key);
    }

    DataResponse response;
    response.setArray(results);
    response.setSuccess(!results.isEmpty());

    onResponse(response);
}

void DataController::fetchManyObjects(const DataRequest &request, const DataRequestCallback &onProgress, const DataResponseCallback &onResponse)
{
    const QString q = request.query();
    const QVariantMap filters = request.filters();

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

    int currentPage = request.page();
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

    response.setTitle("Error");
    response.setText("Hey ho");

    QTimer::singleShot(NETWORK_LATENCY, qApp, [onResponse, response] { onResponse(response); });
}

void DataController::fetchOneObject(const DataRequest &request, const DataRequestCallback &onProgress, const DataResponseCallback &onResponse)
{
    const int id = request.object().integer("id");

    DataResponse response;
    if (id <= m_objects.size()) {
        response.setObject(m_objects.at(id - 1));
        response.setSuccess(true);
    }

    QTimer::singleShot(3000, qApp, [onResponse, response] { onResponse(response); });
}

void DataController::addOneObject(const DataRequest &request, const DataRequestCallback &onProgress, const DataResponseCallback &onResponse)
{
    Jsoner::Object object = request.object();
    object.put("id", m_objects.size());
    m_objects.append(object);

    DataResponse response;
    response.setObject(object);
    response.setSuccess(true);

    QTimer::singleShot(NETWORK_LATENCY, qApp, [onResponse, response] { onResponse(response); });
}

void DataController::editOneObject(const DataRequest &request, const DataRequestCallback &onProgress, const DataResponseCallback &onResponse)
{
    const int id = request.object().integer("id");

    DataResponse response;
    if (id <= m_objects.size()) {
        m_objects.replace(id - 1, request.object());
        response.setObject(request.object());
        response.setSuccess(true);
    }

    QTimer::singleShot(NETWORK_LATENCY, qApp, [onResponse, response] { onResponse(response); });
}

void DataController::deleteOneObject(const DataGate::DataRequest &request, const DataGate::DataRequestCallback &onProgress, const DataGate::DataResponseCallback &onResponse)
{
    const int id = request.object().integer("id");

    DataResponse response;
    if (id <= m_objects.size()) {
        m_objects.removeAt(id - 1);
        response.setObject(request.object());
        response.setSuccess(true);
    }

    QTimer::singleShot(NETWORK_LATENCY, qApp, [onResponse, response] { onResponse(response); });
}

void DataController::deleteManyObjects(const DataRequest &request, const DataRequestCallback &onProgress, const DataResponseCallback &onResponse)
{
    const int id = request.object().integer("id");
    auto it = std::find_if(m_objects.begin(), m_objects.end(), [&id](const QJsonValue &value) {
        return value.toObject().value("id").toInt() == id;
    });

    if (it != m_objects.end()) {
        m_objects.erase(it);
        DataResponse response;
        response.setSuccess(true);
        QTimer::singleShot(request.object().size() * NETWORK_LATENCY, qApp, [onResponse, response] { onResponse(response); });
    } else {
        QTimer::singleShot(NETWORK_LATENCY, qApp, [onResponse] { onResponse(DataResponse()); });
    }
}
