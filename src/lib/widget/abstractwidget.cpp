#include "abstractwidget.h"
#include "abstractwidget_p.h"

#include <Widgetry/widgetrequest.h>

#include <QtCore/qsettings.h>

#include <QtGui/qevent.h>

#include <QtWidgets/qdialog.h>

namespace Widgetry {

AbstractWidget::~AbstractWidget()
{
}

QByteArray AbstractWidget::id() const
{
    return d_ptr->id;
}

void AbstractWidget::setId(const QByteArray &id)
{
    if (d_ptr->id == id)
        return;

    d_ptr->id = id;
    processIdChange(id);
}

QIcon AbstractWidget::icon() const
{
    return d_ptr->widget->windowIcon();
}

void AbstractWidget::setIcon(const QIcon &icon)
{
    d_ptr->widget->setWindowIcon(icon);
}

QString AbstractWidget::title() const
{
    return d_ptr->widget->windowTitle();
}

void AbstractWidget::setTitle(const QString &title)
{
    d_ptr->widget->setWindowTitle(title);
}

QAction *AbstractWidget::action() const
{
    return d_ptr->action;
}

void AbstractWidget::setAction(QAction *action)
{
    if (d_ptr->action == action)
        return;

    QAction *oldAction = d_ptr->action;
    d_ptr->action = action;
    processActionChange(action, oldAction);
}

bool AbstractWidget::isOperationSupported(const QString &operation) const
{
    Q_UNUSED(operation);
    return false;
}

QStringList AbstractWidget::supportedOperations() const
{
    QStringList operations = availableOperations();
    operations.removeIf([this](const QString &operation) {
        return !isOperationSupported(operation);
    });
    return operations;
}

QStringList AbstractWidget::availableOperations() const
{
    return QStringList();
}

QVariant AbstractWidget::operate(const QString &operation, bool *success)
{
    return operate(operation, QVariantMap(), success);
}

QVariant AbstractWidget::operate(const QString &operation, const QVariant &parameter, bool *success)
{
    const QVariantMap parameters = { { "parameter", parameter } };
    return operate(operation, parameters, success);
}

QVariant AbstractWidget::operate(const QString &operation, const QVariantMap &parameters, bool *success)
{
    WidgetRequest request(operation);
    request.setParameters(parameters);
    request.setSenderId(d_ptr->id);
    request.setReceiverId(d_ptr->id);

    WidgetResponse response = handleRequest(request);

    if (success)
        *success = response.isSuccess();

    return response.data();
}

void AbstractWidget::sync()
{
    const QStringList allOperations = availableOperations();
    for (const QString &operation : allOperations)
        processOperationSupportChanged(operation, isOperationSupported(operation));
}

void AbstractWidget::loadSettings(QSettings *settings)
{
    d_ptr->widget->restoreGeometry(settings->value("geo").toByteArray());
}

void AbstractWidget::saveSettings(QSettings *settings) const
{
    settings->setValue("geo", d_ptr->widget->saveGeometry());
}

QWidget *AbstractWidget::widget() const
{
    return d_ptr->widget;
}

QDialog *AbstractWidget::dialog() const
{
    if (d_ptr->widget->inherits("QDialog"))
        return static_cast<QDialog *>(d_ptr->widget);
    else
        return nullptr;
}

void AbstractWidget::processIdChange(const QByteArray &id)
{
    Q_UNUSED(id);
}

void AbstractWidget::prepareUi(bool firstShow)
{
    Q_UNUSED(firstShow);
}

void AbstractWidget::cleanupUi()
{
}

void AbstractWidget::translateUi(bool full)
{
    Q_UNUSED(full);
}

void AbstractWidget::requestOperation(const QString &name, const QByteArray &target)
{
    WidgetRequest request(name);
    request.setSenderId(d_ptr->id);
    request.setReceiverId(target);
    processRequestDispatch(request);
}

void AbstractWidget::requestOperation(WidgetRequest &request)
{
    request.setSenderId(d_ptr->id);
    processRequestDispatch(request);
}

void AbstractWidget::processRequestDispatch(const WidgetRequest &request)
{
    // ToDo: impl dispatch logic
}

WidgetResponse AbstractWidget::handleRequest(const WidgetRequest &request)
{
    return WidgetResponse::fromRequest(request);
}

void AbstractWidget::handleResponse(const WidgetRequest &request, const WidgetResponse &response)
{
    Q_UNUSED(request);
    Q_UNUSED(response);
}

AbstractWidget::AbstractWidget(AbstractWidgetPrivate *d)
    : d_ptr(d)
{
}

AbstractWidgetPrivate::AbstractWidgetPrivate(AbstractWidget *q, QWidget *widget)
    : q_ptr(q)
    , widget(widget == nullptr ? reinterpret_cast<QWidget *>(q) : widget)
    , firstShow(true)
{
}

AbstractWidgetPrivate::~AbstractWidgetPrivate()
{
}

void AbstractWidgetPrivate::updateLayout()
{
    updateLayout(widget->isWindow());
}

void AbstractWidgetPrivate::updateLayout(bool window)
{
    if (window)
        widget->setContentsMargins(QMargins(9, 9, 9, 9));
    else
        widget->setContentsMargins(QMargins());
}

bool AbstractWidgetPrivate::processEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Show:
        if (firstShow) {
            q_ptr->prepareUi(true);
            q_ptr->translateUi(false);
            firstShow = false;
        } else {
            q_ptr->prepareUi(false);
        }
        return true;

    case QEvent::Hide:
        q_ptr->cleanupUi();
        return true;

    case QEvent::LanguageChange:
        q_ptr->translateUi(true);
        return true;

    default:
        return false;
    }
}

} // namespace Widgetry
