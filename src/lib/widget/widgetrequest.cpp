#include "widgetrequest.h"

#include <QtCore/qvariant.h>

using namespace DataGate;

namespace Widgetry {

WidgetRequest::WidgetRequest(const QString &name)
{
    DataRequest::setParameter("name", name);
}

WidgetRequest::WidgetRequest(const WidgetRequest &other)
    : DataRequest(other)
{
}

WidgetRequest::WidgetRequest(WidgetRequest &&other)
    : DataRequest(std::move(other))
{
}

WidgetRequest::~WidgetRequest()
{
}

QString WidgetRequest::name() const
{
    return DataRequest::parameter("name").toString();
}

QVariant WidgetRequest::parameter() const
{
    const QVariantMap parameters = DataRequest::parameters();
    const QStringList names = parameters.keys();
    return names.isEmpty() ? QVariant() : parameters.value(names.constFirst());
}

QVariant WidgetRequest::parameter(const QString &name) const
{
    return DataRequest::parameter(name);
}

QByteArray WidgetRequest::senderId() const
{
    return DataRequest::parameter("senderId").toByteArray();
}

void WidgetRequest::setSenderId(const QByteArray &id)
{
    DataRequest::setParameter("senderId", id);
}

QByteArray WidgetRequest::receiverId() const
{
    return DataRequest::parameter("receiverId").toByteArray();
}

void WidgetRequest::setReceiverId(const QByteArray &id)
{
    DataRequest::setParameter("receiverId", id);
}

QString WidgetResponse::name() const
{
    return DataResponse::data("name").toString();
}

QVariant WidgetResponse::data(const QString &name) const
{
    return DataResponse::data(name);
}

QVariant WidgetResponse::data() const
{
    const QStringList names = DataResponse::dataNames();
    return names.isEmpty() ? QVariant() : DataResponse::data(names.constFirst());
}

QByteArray WidgetResponse::senderId() const
{
    return DataResponse::data("senderId").toByteArray();
}

void WidgetResponse::setSenderId(const QByteArray &id)
{
    DataResponse::setData("senderId", id);
}

QByteArray WidgetResponse::receiverId() const
{
    return DataResponse::data("receiverId").toByteArray();
}

void WidgetResponse::setReceiverId(const QByteArray &id)
{
    DataResponse::setData("receiverId", id);
}

WidgetResponse WidgetResponse::fromRequest(const WidgetRequest &request)
{
    WidgetResponse response;
    response.setData("name", request.name());
    response.setData("senderId", request.receiverId());
    response.setData("receiverId", request.senderId());
    return response;
}

} // namespace Widgetry
