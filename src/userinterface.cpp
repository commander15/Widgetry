#include "userinterface.h"
#include "userinterface_p.h"

namespace Widgetry {

UserInterface::UserInterface(QWidget *parent, Qt::WindowFlags flags)
    : UserInterface(new UserInterfacePrivate(this), parent, flags)
{
}

UserInterface::UserInterface(const QByteArray &id, QWidget *parent, Qt::WindowFlags flags)
    : UserInterface(new UserInterfacePrivate(id, this), parent, flags)
{
}

UserInterface::UserInterface(UserInterfacePrivate *d, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
}

UserInterface::~UserInterface()
{
}

QByteArray UserInterface::id() const
{
    return d_ptr->id;
}

bool UserInterface::isOperationSupported(int operation) const
{
    Q_UNUSED(operation);
    return false;
}

QVariant UserInterface::operate(int operation)
{
    return operate(operation, QVariantHash());
}

QVariant UserInterface::operate(int operation, const QVariant &parameter)
{
    QVariantHash parameters;
    parameters.insert("parameter", parameter);
    return operate(operation, parameters);
}

QVariant UserInterface::operate(int operation, const QVariantHash &parameters)
{
    if (!isOperationSupported(operation))
        return QVariant();

    QVariantHash result = handleOperation(operation, parameters);
    if (result.isEmpty())
        return QVariant();
    else if (result.size() == 1)
        return result.value(result.keys().first());
    else
        return result;
}

void UserInterface::sync()
{
    // No-op
}

QVariantHash UserInterface::handleOperation(int operation, const QVariantHash &parameters)
{
    // No-op

    Q_UNUSED(operation);
    Q_UNUSED(parameters);
    return QVariantHash();
}

UserInterfacePrivate::UserInterfacePrivate(UserInterface *q)
    : q_ptr(q)
{
}

UserInterfacePrivate::UserInterfacePrivate(const QByteArray &id, UserInterface *q)
    : q_ptr(q)
    , id(id)
{
}

} // namespace Widgetry
