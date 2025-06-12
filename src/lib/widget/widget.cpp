#include "widget.h"
#include "widget_p.h"

#include <Widgetry/widgetoperation.h>

#include <QtCore/qsettings.h>

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
    setIcon(QIcon(":/widgetry/icons/widgetry.png"));

    connect(this, &QWidget::windowIconChanged, this, &Widget::iconChanged);
    connect(this, &QWidget::windowTitleChanged, this, &Widget::titleChanged);
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
    if (action) {
        action->setIcon(icon());
        action->setText(title());
    }

    if (d_ptr->action) {
        disconnect(this, &Widget::iconChanged, d_ptr->action, &QAction::setIcon);
        disconnect(this, &Widget::titleChanged, d_ptr->action, &QAction::setText);
    }

    d_ptr->action = action;

    if (d_ptr->action) {
        connect(this, &Widget::iconChanged, d_ptr->action, &QAction::setIcon);
        connect(this, &Widget::titleChanged, d_ptr->action, &QAction::setText);
    }
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
    if (!isOperationSupported(operation)) {
        if (success)
            *success = false;
        return QVariant();
    }

    WidgetOperation op(operation);
    op.setParameters(parameters);
    op.setSenderId(id());
    op.setReceiverId(id());

    bool successFallback = false;
    return handleOperationRequest(op, success ? *success : successFallback);
}

void Widget::sync()
{
    const QStringList availableOperations = this->availableOperations();
    for (const QString &operation : availableOperations)
        emit operationSupportChanged(operation, isOperationSupported(operation));
}

void Widget::loadSettings(QSettings *settings)
{
    restoreGeometry(settings->value("geo").toByteArray());
}

void Widget::saveSettings(QSettings *settings) const
{
    settings->setValue("geo", saveGeometry());
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

void Widget::requestManagerOperation(const QString &name)
{
    WidgetOperation operation(name);
    operation.setReceiverId(QByteArrayLiteral("Widgetry::WidgetManager"));
    requestOperation(operation);
}

void Widget::requestManagerOperation(WidgetOperation &operation)
{
    operation.setReceiverId(QByteArrayLiteral("Widgetry::WidgetManager"));
    requestOperation(operation);
}

void Widget::requestOperation(const QString &name, const QByteArray &target)
{
    WidgetOperation operation(name);
    operation.setReceiverId(target);
    requestOperation(operation);
}

void Widget::requestOperation(WidgetOperation &operation)
{
    operation.setSenderId(id());
    emit operationRequested(operation);
}

QVariant Widget::handleOperationRequest(const WidgetOperation &operation, bool &success)
{
    // No-op
    Q_UNUSED(operation);
    success = false;
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
    if (event->type() == QEvent::LanguageChange) {
        event->accept();
        translateUi(true);
    }

    QWidget::changeEvent(event);
}

WidgetPrivate::WidgetPrivate(Widget *q, const QByteArray &id)
    : q_ptr(q)
    , id(id)
    , action(nullptr)
{
}

} // namespace Widgetry
