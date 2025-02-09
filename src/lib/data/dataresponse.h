#ifndef WIDGETRY_DATARESPONSE_H
#define WIDGETRY_DATARESPONSE_H

#include <Widgetry/global.h>

#include <QtCore/qshareddata.h>

namespace Jsoner {
class Object;
class Array;
}

namespace Widgetry {

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

    Jsoner::Object object() const;
    void setObject(const Jsoner::Object &object);

    Jsoner::Array objects() const;
    void setObjects(const Jsoner::Array &objects);

    int page() const;
    void setPage(int page);

    int pageCount() const;
    void setPageCount(int count);

    bool hasData(const QString &name) const;
    QVariant data(const QString &name) const;
    void setData(const QString &name, const QVariant &value);
    QStringList dataNames() const;

    bool isSuccess() const;
    void setSuccess(bool success);

private:
    QSharedDataPointer<DataResponseData> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DATARESPONSE_H
