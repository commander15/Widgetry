#include "userinterface.h"
#include "userinterface_p.h"

#include <Widgetry/operation.h>
#include <Widgetry/interfaceserver.h>

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
    , d_ptr(d)
{
    connect(this, &QWidget::windowIconChanged, this, &UserInterface::iconChanged);
    connect(this, &QWidget::windowTitleChanged, this, &UserInterface::titleChanged);
}

UserInterface::~UserInterface()
{
}

QByteArray UserInterface::id() const
{
    return d_ptr->id;
}

QIcon UserInterface::icon() const
{
    return windowIcon();
}

QString UserInterface::title() const
{
    return windowTitle();
}

QAction *UserInterface::action() const
{
    return d_ptr->action;
}

bool UserInterface::isOperationSupported(const QString &operation) const
{
    Q_UNUSED(operation);
    return false;
}

QVariant UserInterface::operate(const QString &operation)
{
    return operate(operation, QVariantHash());
}

QVariant UserInterface::operate(const QString &operation, const QVariant &parameter)
{
    QVariantHash parameters;
    parameters.insert("parameter", parameter);
    return operate(operation, parameters);
}

QVariant UserInterface::operate(const QString &operation, const QVariantHash &parameters)
{
    if (!isOperationSupported(operation))
        return QVariant();

    Operation op(operation);
    op.setParameters(parameters);
    op.setSenderId(id());
    op.setReceiverId(id());

    if (!handleOperation(&op))
        return QVariant();

    const QVariantHash result = op.results();
    if (result.isEmpty())
        return QVariant();
    else if (result.size() == 1)
        return result.value(result.keys().first());
    else
        return result;
}

void UserInterface::setIcon(const QIcon &icon)
{
    setWindowIcon(icon);
}

void UserInterface::setTitle(const QString &title)
{
    setWindowTitle(title);
}

void UserInterface::setAction(QAction *action)
{
    if (d_ptr->action) {
        // Disconnect old action signals
        disconnect(this, &QWidget::windowTitleChanged, d_ptr->action, &QAction::setText);
        disconnect(this, &QWidget::windowIconChanged, d_ptr->action, &QAction::setIcon);

        d_ptr->action->deleteLater();
    }

    action->setText(windowTitle());
    action->setIcon(windowIcon());
    action->setParent(this);

    // Connect action to this widget
    connect(this, &QWidget::windowTitleChanged, action, &QAction::setText);
    connect(this, &QWidget::windowIconChanged, action, &QAction::setIcon);

    d_ptr->action = action;
}

void UserInterface::sync()
{
    // No-op
}

bool UserInterface::handleOperation(Operation *operation)
{
    // No-op
    Q_UNUSED(operation);
    return false;
}

void UserInterface::requestServerOperation(const Operation &operation)
{
    Operation op(operation);
    op.setSenderId(id());
    op.setReceiverId(WIDGETRY_INTERFACE_SERVER);
    emit operationRequested(op);
}

void UserInterface::requestOperation(const Operation &operation)
{
    Operation op(operation);
    op.setSenderId(id());
    emit operationRequested(op);
}

UserInterface *UserInterface::findInterface(const QByteArray &id) const
{
    return nullptr;
}

UserInterfacePrivate::UserInterfacePrivate(UserInterface *q)
    : UserInterfacePrivate(QByteArray(), q)
{
}

UserInterfacePrivate::UserInterfacePrivate(const QByteArray &id, UserInterface *q)
    : q_ptr(q)
    , id(id)
    , action(nullptr)
{
}

} // namespace Widgetry
