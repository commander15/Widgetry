#ifndef WIDGETRY_USERINTERFACE_H
#define WIDGETRY_USERINTERFACE_H

#include <Widgetry/global.h>

#include <QtWidgets/qwidget.h>

namespace Widgetry {

class UserInterfacePrivate;
class WIDGETRY_EXPORT UserInterface : public QWidget
{
    Q_OBJECT

public:
    UserInterface(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    UserInterface(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~UserInterface();

    QByteArray id() const;

    virtual bool isOperationSupported(int operation) const;
    QVariant operate(int operation);
    QVariant operate(int operation, const QVariant &parameter);
    QVariant operate(int operation, const QVariantHash &parameters);

public slots:
    virtual void sync();

signals:
    void operationSupportChanged(int operation, bool supported);

protected:
    UserInterface(UserInterfacePrivate *d, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    virtual QVariantHash handleOperation(int operation, const QVariantHash &parameters);

    QScopedPointer<UserInterfacePrivate> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_USERINTERFACE_H
