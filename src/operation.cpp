#include "operation.h"
#include "operation_p.h"

namespace Widgetry {

Operation::Operation(const QString &name)
    : d_ptr(new OperationData())
{
}

Operation::Operation(const Operation &other)
    : d_ptr(other.d_ptr)
{
}

Operation::Operation(Operation &&other)
    : d_ptr(std::move(other.d_ptr))
{
}

Operation::~Operation()
{
}

QString Operation::name() const
{
    return d_ptr->name;
}

QByteArray Operation::senderId() const
{
    return d_ptr->senderId;
}

void Operation::setSenderId(const QByteArray &id)
{
    d_ptr->senderId = id;
}

QByteArray Operation::receiverId() const
{
    return d_ptr->receiverId;
}

void Operation::setReceiverId(const QByteArray &id)
{
    d_ptr->receiverId = id;
}

bool Operation::isSuccess() const
{
    return d_ptr->success;
}

void Operation::setSuccess(bool success)
{
    d_ptr->success = success;
}

QVariantHash Operation::results() const
{
    return d_ptr->results;
}

void Operation::setResults(const QVariantHash &results)
{
    d_ptr->results = results;
}

} // namespace Widgetry
