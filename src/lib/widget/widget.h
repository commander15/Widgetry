#ifndef WIDGETRY_WIDGET_H
#define WIDGETRY_WIDGET_H

#include <Widgetry/global.h>

#include <QtWidgets/qwidget.h>

class QSettings;

namespace Widgetry {

class WidgetOperation;

class WidgetPrivate;
class WIDGETRY_EXPORT Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    Widget(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~Widget();

    QByteArray id() const;
    void setId(const QByteArray &id);

    QIcon icon() const;
    void setIcon(const QIcon &icon);
    Q_SIGNAL void iconChanged(const QIcon &icon);

    QString title() const;
    void setTitle(const QString &title);
    Q_SIGNAL void titleChanged(const QString &title);

    QAction *action() const;
    void setAction(QAction *action);

    virtual bool isOperationSupported(const QString &operation) const;
    QStringList supportedOperations() const;
    virtual QStringList availableOperations() const;
    Q_SIGNAL void operationSupportChanged(const QString &operation, bool supported);
    Q_SIGNAL void operationRequested(const WidgetOperation &operation);

    QVariant operate(const QString &operation, bool *success = nullptr);
    QVariant operate(const QString &operation, const QVariant &parameter, bool *success = nullptr);
    QVariant operate(const QString &operation, const QVariantHash &parameters, bool *success = nullptr);

    Q_SLOT virtual void sync();

    virtual void loadSettings(QSettings *settings);
    virtual void saveSettings(QSettings *settings) const;

protected:
    Widget(WidgetPrivate *d, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    virtual void prepareUi();
    virtual void cleanupUi();
    virtual void translateUi(bool full = true);

    void requestManagerOperation(const QString &name);
    void requestManagerOperation(WidgetOperation &operation);

    void requestOperation(const QString &name, const QByteArray &target);
    void requestOperation(WidgetOperation &operation);

    virtual QVariant handleOperationRequest(const WidgetOperation &operation, bool &success);
    virtual void handleOperationResult(const WidgetOperation &operation, const QVariantHash &result, bool success);

    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void changeEvent(QEvent *event) override;

    QScopedPointer<WidgetPrivate> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_WIDGET_H
