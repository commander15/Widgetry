#include "widget.h"
#include "widget_p.h"

#include <Widgetry/widgetoperation.h>

#include <QtGui/qevent.h>

namespace Widgetry {

Widget::Widget(const QByteArray &id, QWidget *parent, Qt::WindowFlags flags)
    : Widget(new WidgetPrivate(this, id), parent, flags)
{
}

Widget::Widget(QWidget *parent, Qt::WindowFlags flags)
    : Widget(new WidgetPrivate(this, QByteArray()), parent, flags)
{
}

Widget::Widget(WidgetPrivate *d, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
    , d_ptr(d)
{
}

Widget::~Widget()
{
}

QByteArray Widget::id() const
{
    return d_ptr->id;
}

void Widget::setId(const QByteArray &id)
{
    d_ptr->id = id;
}

QIcon Widget::icon() const
{
    return windowIcon();
}

void Widget::setIcon(const QIcon &icon)
{
    setWindowIcon(icon);
}

QString Widget::title() const
{
    return windowTitle();
}

void Widget::setTitle(const QString &title)
{
    setWindowTitle(title);
}

QAction *Widget::action() const
{
    return d_ptr->action;
}

void Widget::setAction(QAction *action)
{
    d_ptr->action = action;
}

bool Widget::isOperationSupported(const QString &operation) const
{
    return availableOperations().contains(operation);
}

QStringList Widget::supportedOperations() const
{
    QStringList operations = availableOperations();
    operations.removeIf([this](const QString &operation) {
        return !isOperationSupported(operation);
    });
    return operations;
}

QStringList Widget::availableOperations() const
{
    return QStringList();
}

QVariant Widget::operate(const QString &operation, bool *success)
{
    return operate(operation, QVariantHash(), success);
}

QVariant Widget::operate(const QString &operation, const QVariant &parameter, bool *success)
{
    QVariantHash parameters;
    parameters.insert("parameter", parameter);
    return operate(operation, parameters, success);
}

QVariant Widget::operate(const QString &operation, const QVariantHash &parameters, bool *success)
{
    if (!isOperationSupported(operation))
        return QVariant();

    WidgetOperation op(operation);
    op.setParameters(parameters);
    op.setSenderId(id());
    op.setReceiverId(id());

    bool successFallback = false;
    return handleOperation(op, success ? success : &successFallback);
}

void Widget::sync()
{
    const QStringList availableOperations = this->availableOperations();
    for (const QString &operation : availableOperations)
        emit operationSupportChanged(operation, isOperationSupported(operation));
}

void Widget::prepareUi()
{
}

void Widget::cleanupUi()
{
}

void Widget::translateUi(bool full)
{
    Q_UNUSED(full);
}

void Widget::requestServerOperation(const QString &name)
{
    WidgetOperation operation(name);
    operation.setReceiverId(QByteArray());
    requestOperation(operation);
}

void Widget::requestServerOperation(WidgetOperation &operation)
{
    operation.setReceiverId(QByteArray());
    requestOperation(operation);
}

void Widget::requestOperation(const QString &name)
{
    WidgetOperation operation(name);
    requestOperation(operation);
}

void Widget::requestOperation(WidgetOperation &operation)
{
    operation.setSenderId(id());
    emit operationRequested(operation);
}

QVariant Widget::handleOperation(const WidgetOperation &operation, bool *success)
{
    // No-op
    Q_UNUSED(operation);
    *success = false;
    return QVariant();
}

void Widget::handleOperationResult(const WidgetOperation &operation, const QVariantHash &result, bool success)
{
    // No-op
    Q_UNUSED(operation);
    Q_UNUSED(result);
    Q_UNUSED(success)
}

void Widget::showEvent(QShowEvent *event)
{
    prepareUi();
    translateUi(false);
    QWidget::showEvent(event);
}

void Widget::hideEvent(QHideEvent *event)
{
    cleanupUi();
    QWidget::hideEvent(event);
}

void Widget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        translateUi(true);
    QWidget::changeEvent(event);
}

WidgetPrivate::WidgetPrivate(Widget *q, const QByteArray &id)
    : q_ptr(q)
    , id(id)
    , action(nullptr)
{
}

} // namespace Widgetry
