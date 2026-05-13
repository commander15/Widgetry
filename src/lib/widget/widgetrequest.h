#ifndef WIDGETRY_REQUEST_H
#define WIDGETRY_REQUEST_H

#include <Widgetry/global.h>

#include <DataGate/datarequest.h>
#include <DataGate/dataresponse.h>

#include <QtCore/qshareddata.h>

namespace Widgetry {

class WIDGETRY_EXPORT WidgetRequest : public DataGate::DataRequest
{
public:
    WidgetRequest(const QString &name);
    WidgetRequest(const WidgetRequest &other);
    WidgetRequest(WidgetRequest &&other);
    ~WidgetRequest();

    QString name() const;

    QVariant parameter() const;
    QVariant parameter(const QString &name) const;

    QByteArray senderId() const;
    void setSenderId(const QByteArray &id);

    QByteArray receiverId() const;
    void setReceiverId(const QByteArray &id);
};

class WIDGETRY_EXPORT WidgetResponse : public DataGate::DataResponse
{
public:
    QString name() const;

    QVariant data(const QString &name) const;
    QVariant data() const;

    QByteArray senderId() const;
    void setSenderId(const QByteArray &id);

    QByteArray receiverId() const;
    void setReceiverId(const QByteArray &id);

    static WidgetResponse fromRequest(const WidgetRequest &request);
};

} // namespace Widgetry

#endif // WIDGETRY_REQUEST_H
