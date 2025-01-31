#ifndef WIDGETRY_ABSTRACTDATACONTROLLER_H
#define WIDGETRY_ABSTRACTDATACONTROLLER_H

#include <Widgetry/global.h>

#include <Jsoner/object.h>

#include <QtCore/qobject.h>

namespace Widgetry {

#include <QString>
#include <QVariantHash>
#include <QSharedData>

class DataQueryData;
class WIDGETRY_EXPORT DataQuery {
public:
    DataQuery();
    DataQuery(const DataQuery &other);
    ~DataQuery();

    DataQuery &operator=(const DataQuery &other);

    QString query() const;
    void setQuery(const QString &query);

    QVariantHash filters() const;
    void setFilters(const QVariantHash &filters);

    QVariantHash parameters() const;
    void setParameters(const QVariantHash &parameters);

    int page() const;
    void setPage(int page);

private:
    QSharedDataPointer<DataQueryData> d_ptr;
};

class DataResponseData;
class WIDGETRY_EXPORT DataResponse
{
public:
    DataResponse();
    DataResponse(const DataResponse &other);
    ~DataResponse();

    DataResponse &operator=(const DataResponse &other);

    QString text() const;
    void setText(const QString &text);

    QString informativeText() const;
    void setInformativeText(const QString &text);

    QString detailedText() const;
    void setDetailedText(const QString &text);

    int page() const;
    void setPage(int page);

    int pageCount() const;
    void setPageCount(int count);

    QVariant data(const QString &name) const;
    void setData(const QString &name, const QVariant &value);
    QStringList dataNames() const;

private:
    QSharedPointer<DataResponseData> d_ptr;
};

class WIDGETRY_EXPORT AbstractDataController : public QObject
{
    Q_OBJECT

public:
    enum RequestType {
        UnknownRequest,
        FetchRequest,
        AddRequest,
        EditRequest,
        DeleteRequest
    };

    explicit AbstractDataController(QObject *parent = nullptr);
    virtual ~AbstractDataController() = default;

public slots:
    virtual void fetchObjects(const DataQuery &query) = 0;
    virtual void fetchObject(const Jsoner::Object &object, int targetRequestType) = 0;
    virtual void addObject(const Jsoner::Object &object) = 0;
    virtual void editObject(const Jsoner::Object &object) = 0;
    virtual void deleteObjects(const Jsoner::Array &objects) = 0;

signals:
    void objectsFetched(const Jsoner::Array &objects, const DataResponse &response);
    void objectFetched(const Jsoner::Object &object, const DataResponse &response, int targetRequestType);
    void objectAdded(const Jsoner::Object &object, const DataResponse &response);
    void objectEdited(const Jsoner::Object &object, const DataResponse &response);
    void objectsDeleted(const Jsoner::Array &objects, const DataResponse &response);

    void errorOccured(int requestType, const Jsoner::Array &objects, const DataResponse &error);
};

} // namespace Widgetry

#endif // WIDGETRY_ABSTRACTDATACONTROLLER_H
