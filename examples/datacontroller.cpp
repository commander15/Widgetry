#include "datacontroller.h"

#include <Widgetry/dataquery.h>
#include <Widgetry/dataresponse.h>

#include <Jsoner/object.h>

#include <QtCore/qrandom.h>

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

void DataController::fetchSomeSuggestions(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    Jsoner::Array results;
    for (const QJsonValue &value : m_objects) {
        const QString key = value.toObject().value("name").toString();
        if (key.startsWith(query.query(), Qt::CaseInsensitive))
            results.append(key);
    }

    Widgetry::DataResponse response;
    response.setObjects(results);
    response.setSuccess(!results.isEmpty());

    onResponse(response);
}

void DataController::fetchManyObjects(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    Jsoner::Array objects;
    for (const QJsonValue &value : m_objects) {
        const Jsoner::Object object = value.toObject();

        const QString q = query.query();
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

    Widgetry::DataResponse response;
    if (query.page() > 0 && query.page() <= m_objects.size()) {
        response.setObjects(objects);
        response.setPage(query.page());
        response.setPageCount(pageCount);
        response.setSuccess(true);
    }
    onResponse(response);
}

void DataController::fetchOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    const int id = query.object().integer("id");

    Widgetry::DataResponse response;
    if (id <= m_objects.size()) {
        response.setObject(m_objects.at(id - 1));
        response.setSuccess(true);
    }
    onResponse(response);
}

void DataController::addOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    Jsoner::Object object = query.object();
    object.put("id", m_objects.size());
    m_objects.append(object);

    Widgetry::DataResponse response;
    response.setObject(object);
    response.setSuccess(true);
    onResponse(response);
}

void DataController::editOneObject(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    const int id = query.object().integer("id");

    Widgetry::DataResponse response;
    if (id <= m_objects.size()) {
        m_objects.replace(id - 1, query.object());
        response.setObject(query.object());
        response.setSuccess(true);
    }
    onResponse(response);
}

void DataController::deleteManyObjects(const Widgetry::DataQuery &query, const Widgetry::DataQueryProgressCallback &onProgress, const Widgetry::DataQueryResponseCallback &onResponse)
{
    onResponse(Widgetry::DataResponse());
}
