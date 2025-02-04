#ifndef WIDGETRY_USERINTERFACE_H
#define WIDGETRY_USERINTERFACE_H

#include <Widgetry/global.h>

#include <QtWidgets/qwidget.h>

namespace Widgetry {

class Operation;

class UserInterfacePrivate;
class WIDGETRY_EXPORT UserInterface : public QWidget
{
    Q_OBJECT

public:
    UserInterface(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    UserInterface(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~UserInterface();

    QByteArray id() const;
    QIcon icon() const;
    QString title() const;
    QAction *action() const;

    virtual bool isOperationSupported(const QString &operation) const;
    QStringList supportedOperations() const;
    virtual QStringList availableOperations() const;

    QVariant operate(const QString &operation);
    QVariant operate(const QString &operation, const QVariant &parameter);
    QVariant operate(const QString &operation, const QVariantHash &parameters);

public slots:
    void setIcon(const QIcon &icon);
    void setTitle(const QString &title);
    void setAction(QAction *action);

    virtual void sync();

signals:
    void iconChanged(const QIcon &icon);
    void titleChanged(const QString &title);

    void operationSupportChanged(const QString &operation, bool supported);
    void operationRequested(const Operation &operation);

protected:
    UserInterface(UserInterfacePrivate *d, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    virtual void prepareUi();
    virtual void cleanupUi();
    virtual void translateUi(bool full = true);

    virtual bool handleOperation(Operation *operation);
    virtual void handleOperationResult(const Operation &operation);

    void requestServerOperation(const Operation &operation);
    void requestOperation(const Operation &operation);

    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void changeEvent(QEvent *event) override;

    QScopedPointer<UserInterfacePrivate> d_ptr;

private:
    Q_DISABLE_COPY(UserInterface)

    friend class InterfaceServer;
};

} // namespace Widgetry

#endif // WIDGETRY_USERINTERFACE_H
