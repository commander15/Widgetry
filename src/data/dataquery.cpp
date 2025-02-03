#include "dataquery.h"

#include <Jsoner/object.h>
#include <Jsoner/array.h>

#include <QtCore/qstring.h>
#include <QtCore/qvarianthash.h>

namespace Widgetry {

class DataQueryData : public QSharedData
{
public:
    QString query;
    QVariantHash filters;
    int page = 1;
    Jsoner::Array objects;
    QVariantHash parameters;
};

DataQuery::DataQuery()
    : d_ptr(new DataQueryData)
{
}

DataQuery::DataQuery(const Jsoner::Object &object)
    : d_ptr(new DataQueryData)
{
    d_ptr->objects.append(object);
}

DataQuery::DataQuery(const Jsoner::Array &objects)
    : d_ptr(new DataQueryData)
{
    d_ptr->objects = objects;
}

DataQuery::DataQuery(const DataQuery &other) = default;

DataQuery::~DataQuery() = default;

DataQuery &DataQuery::operator=(const DataQuery &other) = default;

QString DataQuery::query() const {
    return d_ptr->query;
}

void DataQuery::setQuery(const QString &query) {
    d_ptr->query = query;
}

QVariantHash DataQuery::filters() const {
    return d_ptr->filters;
}

void DataQuery::setFilters(const QVariantHash &filters) {
    d_ptr->filters = filters;
}

QVariantHash DataQuery::parameters() const {
    return d_ptr->parameters;
}

void DataQuery::setParameters(const QVariantHash &parameters) {
    d_ptr->parameters = parameters;
}

int DataQuery::page() const {
    return d_ptr->page;
}

void DataQuery::setPage(int page) {
    d_ptr->page = page;
}

Jsoner::Object DataQuery::object() const
{
    return (!d_ptr->objects.isEmpty ? d_ptr->objects.first() : Jsoner::Object());
}

void DataQuery::setObject(const Jsoner::Object &object)
{
    d_ptr->objects = { objects };
}

Jsoner::Array DataQuery::objects() const
{
    return d_ptr->objects;
}

void DataQuery::setObjects(const Jsoner::Array &objects)
{
    d_ptr->objects = objects;
}

} // namespace Widgetry
