#include "interfaceserver.h"
#include "interfaceserver_p.h"

#include <Widgetry/userinterface.h>
#include <Widgetry/operation.h>

#include <QtCore/qcoreapplication.h>
#include <QtCore/qmetaobject.h>

namespace Widgetry {

InterfaceServer::InterfaceServer(QObject *parent) :
    QObject(parent),
    d(new InterfaceServerPrivate(this))
{
}

InterfaceServer::~InterfaceServer()
{
}

InterfaceServer *InterfaceServer::global()
{
    static InterfaceServer *server = nullptr;

    if (!server)
        server = new InterfaceServer(qApp);

    return server;
}

void InterfaceServer::setCurrentIndex(int index)
{
    SimpleInterfaceHolder::setCurrentIndex(index);
    d->currentInterface = interface(index);
    emit currentIndexChanged(index);
}

void InterfaceServer::sync()
{
    for (int i(0); i < count(); ++i)
        QMetaObject::invokeMethod(interface(i), &UserInterface::sync, Qt::QueuedConnection);
}

void InterfaceServer::registerInterface(UserInterface *interface)
{
    connect(interface, &UserInterface::iconChanged, this, &InterfaceServer::processIconChange);
    connect(interface, &UserInterface::titleChanged, this, &InterfaceServer::processTitleChange);
    connect(interface, &UserInterface::operationSupportChanged, this, &InterfaceServer::processOperationSupportChange);
    connect(interface, &UserInterface::operationRequested, this, &InterfaceServer::processRequestedOperation);
}

void InterfaceServer::unregisterInterface(UserInterface *interface)
{
    disconnect(interface, &UserInterface::iconChanged, this, &InterfaceServer::processIconChange);
    disconnect(interface, &UserInterface::titleChanged, this, &InterfaceServer::processTitleChange);
    disconnect(interface, &UserInterface::operationSupportChanged, this, &InterfaceServer::processOperationSupportChange);
    disconnect(interface, &UserInterface::operationRequested, this, &InterfaceServer::processRequestedOperation);
}

void InterfaceServer::processInterfaceChange(UserInterface *from, UserInterface *to)
{
    Q_UNUSED(from);

    emit currentIconChanged(to ? to->icon() : QIcon());
    emit currentTitleChanged(to ? to->title() : QString());

    const QStringList operations = (to ? to->availableOperations() : QStringList());
    for (const QString &operation : operations)
        emit currentOperationSupportChanged(operation, to->isOperationSupported(operation));
}

void InterfaceServer::processIconChange(const QIcon &icon)
{
    if (sender() == d->currentInterface)
        emit currentIconChanged(icon);
}

void InterfaceServer::processTitleChange(const QString &title)
{
    if (sender() == d->currentInterface)
        emit currentTitleChanged(title);
}

void InterfaceServer::processOperationSupportChange(const QString &operation, bool supported)
{
    if (sender() == d->currentInterface)
        emit currentOperationSupportChanged(operation, supported);
}

void InterfaceServer::processRequestedOperation(const Operation &operation)
{
    UserInterface *origin = static_cast<UserInterface *>(sender());

    if (operation.receiverId() == WIDGETRY_INTERFACE_SERVER) {
        Operation op(operation);
        op.setSuccess(processServerOperation(origin, &op));
        origin->handleOperationResult(op);
    } else {
        UserInterface *target = interfaceById(operation.receiverId());
        Operation op(operation);

        if (target) {
            if (target->isOperationSupported(operation.name())) {
                op.setSuccess(target->handleOperation(&op));
                origin->handleOperationResult(op);
            } else {
                emit operationRequested(origin, operation);
            }
        } else {
            op.setSuccess(false);
            origin->handleOperationResult(op);
        }
    }
}

bool InterfaceServer::processServerOperation(UserInterface *origin, Operation *operation)
{
    if (operation->name() == "sync") {
        sync();
        return true;
    }

    return false;
}

InterfaceServerPrivate::InterfaceServerPrivate(InterfaceServer *q) :
    q(q),
    currentInterface(nullptr)
{
}

}
