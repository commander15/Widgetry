#include "widgetoperation.h"
#include "widgetoperation_p.h"

namespace Widgetry {

WidgetOperation::WidgetOperation(const QString &name)
    : d_ptr(new WidgetOperationData())
{
    d_ptr->name = name;
}

WidgetOperation::WidgetOperation(const WidgetOperation &other)
    : d_ptr(other.d_ptr)
{
}

WidgetOperation::WidgetOperation(WidgetOperation &&other)
    : d_ptr(std::move(other.d_ptr))
{
}

WidgetOperation::~WidgetOperation()
{
}

QString WidgetOperation::name() const
{
    return d_ptr->name;
}

QVariant WidgetOperation::parameter() const
{
    if (d_ptr->parameters.size() >= 1)
        return d_ptr->parameters.values().constFirst();
    else
        return QVariant();
}

QVariantHash WidgetOperation::parameters() const
{
    return d_ptr->parameters;
}

void WidgetOperation::setParameters(const QVariantHash &parameters)
{
    d_ptr->parameters = parameters;
}

QByteArray WidgetOperation::senderId() const
{
    return d_ptr->senderId;
}

void WidgetOperation::setSenderId(const QByteArray &id)
{
    d_ptr->senderId = id;
}

QByteArray WidgetOperation::receiverId() const
{
    return d_ptr->receiverId;
}

void WidgetOperation::setReceiverId(const QByteArray &id)
{
    d_ptr->receiverId = id;
}

} // namespace Widgetry
