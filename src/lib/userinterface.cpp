#include "userinterface.h"
#include "userinterface_p.h"

#include <Widgetry/operation.h>
#include <Widgetry/interfaceserver.h>

#include <QtCore/qcoreevent.h>

namespace Widgetry {

UserInterface::UserInterface(QWidget *parent, Qt::WindowFlags flags)
    : UserInterface(new UserInterfacePrivate(this), parent, flags)
{
}

UserInterface::UserInterface(const QByteArray &id, QWidget *parent, Qt::WindowFlags flags)
    : UserInterface(new UserInterfacePrivate(this, id), parent, flags)
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
    return availableOperations().contains(operation);
}

QStringList UserInterface::supportedOperations() const
{
    QStringList operations = availableOperations();
    std::remove_if(operations.begin(), operations.end(), [this](const QString &operation) {
        return !isOperationSupported(operation);
    });
    return operations;
}

QStringList UserInterface::availableOperations() const
{
    return QStringList();
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
    QAction *newAction = action;
    QAction *oldAction = d_ptr->action;

    if (oldAction) {
        // Disconnect old action signals
        disconnect(this, &UserInterface::iconChanged, oldAction, &QAction::setIcon);
        disconnect(this, &UserInterface::titleChanged, oldAction, &QAction::setText);
    }

    if (newAction) {
        action->setText(title());
        action->setIcon(icon());
        action->setParent(this);

        // Connect action to this widget
        connect(this, &UserInterface::iconChanged, newAction, &QAction::setIcon);
        connect(this, &UserInterface::titleChanged, newAction, &QAction::setText);
    }

    d_ptr->action = action;
}

void UserInterface::sync()
{
    const QStringList availableOperations = this->availableOperations();
    for (const QString &operation : availableOperations)
        emit operationSupportChanged(operation, isOperationSupported(operation));
}

void UserInterface::prepareUi()
{
}

void UserInterface::cleanupUi()
{
}

void UserInterface::translateUi(bool full)
{
    Q_UNUSED(full);
}

bool UserInterface::handleOperation(Operation *operation)
{
    // No-op
    Q_UNUSED(operation);
    return false;
}

void UserInterface::handleOperationResult(const Operation &operation)
{
    // No-op
    Q_UNUSED(operation);
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

void UserInterface::showEvent(QShowEvent *event)
{
    prepareUi();
    translateUi(false);
    QWidget::showEvent(event);
}

void UserInterface::hideEvent(QHideEvent *event)
{
    cleanupUi();
    QWidget::hideEvent(event);
}

void UserInterface::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        translateUi();
    QWidget::changeEvent(event);
}

UserInterfacePrivate::UserInterfacePrivate(UserInterface *q)
    : UserInterfacePrivate(q, QByteArray())
{
}

UserInterfacePrivate::UserInterfacePrivate(UserInterface *q, const QByteArray &id)
    : q_ptr(q)
    , id(id)
    , action(nullptr)
{
}

} // namespace Widgetry
