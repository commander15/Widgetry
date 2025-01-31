#include "abstractdatacontroller.h"

namespace Widgetry {

class DataQueryData : public QSharedData {
public:
    QString query;
    QVariantHash filters;
    QVariantHash parameters;
    int page = 1;
};

DataQuery::DataQuery() : d_ptr(new DataQueryData) {}

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

class DataResponseData : public QSharedData
{
public:
    QString text;
    QString informativeText;
    QString detailedText;
    int page = 1;
    int pageCount = 1;
    QVariantHash dataHash;
};

DataResponse::DataResponse()
    : d_ptr(new DataResponseData)
{
}

DataResponse::DataResponse(const DataResponse &other)
    : d_ptr(other.d_ptr)  // Shared pointer, no deep copy needed
{
}

DataResponse::~DataResponse() = default;

DataResponse &DataResponse::operator=(const DataResponse &other)
{
    if (this != &other) {
        d_ptr = other.d_ptr; // Shared pointer, no deep copy needed
    }
    return *this;
}

QString DataResponse::text() const
{
    return d_ptr->text;
}

void DataResponse::setText(const QString &text)
{
    d_ptr->text = text;
}

QString DataResponse::informativeText() const
{
    return d_ptr->informativeText;
}

void DataResponse::setInformativeText(const QString &text)
{
    d_ptr->informativeText = text;
}

QString DataResponse::detailedText() const
{
    return d_ptr->detailedText;
}

void DataResponse::setDetailedText(const QString &text)
{
    d_ptr->detailedText = text;
}

int DataResponse::page() const
{
    return d_ptr->page;
}

void DataResponse::setPage(int page)
{
    d_ptr->page = page;
}

int DataResponse::pageCount() const
{
    return d_ptr->pageCount;
}

void DataResponse::setPageCount(int count)
{
    d_ptr->pageCount = count;
}

QVariant DataResponse::data(const QString &name) const
{
    return d_ptr->dataHash.value(name);
}

void DataResponse::setData(const QString &name, const QVariant &value)
{
    d_ptr->dataHash.insert(name, value);
}

QStringList DataResponse::dataNames() const
{
    return d_ptr->dataHash.keys();
}

AbstractDataController::AbstractDataController(QObject *parent)
    : QObject{parent}
{}

} // namespace Widgetry
