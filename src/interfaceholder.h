#ifndef SYSTEMUS_INTERFACEHOLDER_H
#define SYSTEMUS_INTERFACEHOLDER_H

#include <Widgetry/global.h>

#include <QtCore/qvector.h>

class QIcon;
class QAction;

namespace Widgetry {

class UserInterface;
class InterfaceHolderManager;

class WIDGETRY_EXPORT InterfaceHolder
{
public:
    InterfaceHolder();
    virtual ~InterfaceHolder();

    QIcon currentIcon() const;
    QString currentTitle() const;
    QAction *currentAction() const;
    bool currentOperationSupported(const QString &operation) const;
    virtual UserInterface *currentInterface() const;

    virtual int currentIndex() const = 0;
    virtual void setCurrentIndex(int index) = 0;

    virtual UserInterface *interface(int index) const = 0;
    virtual int count() const = 0;
    virtual int addInterface(UserInterface *interface);
    virtual int insertInterface(int index, UserInterface *interface) = 0;
    virtual void removeInterface(UserInterface *interface) = 0;

    virtual UserInterface *interfaceById(const QByteArray &id) const;
    virtual int indexOf(UserInterface *interface) const;

protected:
    virtual void completeRegistration(InterfaceHolderManager *manager);
    virtual void completeUnregistration(InterfaceHolderManager *manager);

    friend class InterfaceHolderManager;
};

class WIDGETRY_EXPORT SimpleInterfaceHolder : public InterfaceHolder
{
public:
    SimpleInterfaceHolder();
    virtual ~SimpleInterfaceHolder();

    UserInterface *currentInterface() const override;

    int currentIndex() const override;
    void setCurrentIndex(int index) override;

    UserInterface *interface(int index) const override;
    int count() const override;
    int addInterface(UserInterface *interface) override;
    int insertInterface(int index, UserInterface *interface) override;
    void removeInterface(UserInterface *interface) override;

    int indexOf(UserInterface *interface) const override;

protected:
    virtual void processInterfaceChange(UserInterface *from, UserInterface *to);

    virtual void registerInterface(UserInterface *interface);
    virtual void unregisterInterface(UserInterface *interface);

    int _current;
    QVector<UserInterface *> _interfaces;

private:
    bool isValidIndex(int index) const;
};

class WIDGETRY_EXPORT InterfaceHolderManager : public InterfaceHolder
{
public:
    InterfaceHolderManager();
    ~InterfaceHolderManager();

    inline bool hasHolders() const;
    void registerHolder(InterfaceHolder *holder);
    void unregisterHolder(InterfaceHolder *holder);

    UserInterface *currentInterface() const override;

    int currentIndex() const override;
    void setCurrentIndex(int index) override;

    UserInterface *interface(int index) const override;
    int count() const override;
    int addInterface(UserInterface *interface) override;
    int insertInterface(int index, UserInterface *interface) override;
    void removeInterface(UserInterface *interface) override;

    int indexOf(UserInterface *interface) const override;

private:
    QVector<InterfaceHolder *> _holders;
};

}

#endif // SYSTEMUS_INTERFACEHOLDER_H
