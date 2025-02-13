#ifndef WIDGETRY_INTERFACESERVER_H
#define WIDGETRY_INTERFACESERVER_H

#include <Widgetry/global.h>
#include <Widgetry/interfaceholder.h>

#include <QtCore/qobject.h>

#define WIDGETRY_INTERFACE_SERVER "systemus.interface.server"

class QTextDocument;
class QPageLayout;
class QPagedPaintDevice;

namespace Widgetry {

class Operation;

class InterfaceServerPrivate;
class WIDGETRY_EXPORT InterfaceServer : public QObject, public SimpleInterfaceHolder
{
    Q_OBJECT

public:
    explicit InterfaceServer(QObject *parent = nullptr);
    virtual ~InterfaceServer();

    static InterfaceServer *global();

public slots:
    void setCurrentIndex(int index) override;

    void sync();

signals:
    void currentIconChanged(const QIcon &icon);
    void currentTitleChanged(const QString &title);
    void currentOperationSupportChanged(const QString &operation, bool supported);
    void currentIndexChanged(int index);

    void operationRequested(UserInterface *origin, const Operation &operation);

protected:
    void registerInterface(UserInterface *interface) override;
    void unregisterInterface(UserInterface *interface) override;
    void processInterfaceChange(UserInterface *from, UserInterface *to) override;

private:
    Q_SLOT void processIconChange(const QIcon &icon);
    Q_SLOT void processTitleChange(const QString &title);
    Q_SLOT void processOperationSupportChange(const QString &operation, bool supported);
    Q_SLOT void processRequestedOperation(const Operation &operation);

    bool processServerOperation(UserInterface *origin, Operation *operation);

    QScopedPointer<InterfaceServerPrivate> d;
};

}

#endif // WIDGETRY_INTERFACESERVER_H
