#ifndef WIDGETRY_LOGINDIALOG_H
#define WIDGETRY_LOGINDIALOG_H

#include <Widgetry/global.h>
#include <Widgetry/loginedit.h>

#include <QtWidgets/qdialog.h>

namespace DataGate {
class AuthenticationError;
}

namespace Jsoner {
class Object;
}

class QValidator;
class QTimer;

namespace Widgetry {

namespace Ui {
class LoginDialog;
}

class WIDGETRY_EXPORT LoginDialog : public QDialog
{
    Q_OBJECT

public:
    enum IdentifierType {
        UsernameIdentifier = LoginEdit::UsernameLogin,
        EmailIdentifier = LoginEdit::EmailLogin,
    };

    LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QPixmap logo() const;
    QString identifier() const;
    QString password() const;

    IdentifierType identifierType() const;
    void setIdentifierType(IdentifierType type);

    const QValidator *identifierValidator() const;
    void setIdentifierValidator(const QValidator *validator);

    const QValidator *passwordValidator() const;
    void setPasswordValidator(const QValidator *validator);

    void showOnLogOut();
    void setShowOnLogOut(bool show);

public slots:
    void setLogo(const QPixmap &logo);
    void setIdentifier(const QString &identifier);
    void setPassword(const QString &password);

    void setVisible(bool visible) override;

private slots:
    void logIn();
    void showMessage(const Jsoner::Object &user);
    void showError(const DataGate::AuthenticationError &error);
    void hideError();

    void adaptToView(int index);

private:
    Ui::LoginDialog *ui;

    bool m_showOnLogOut;
    QTimer *m_cleanTimer;
};

}

#endif // WIDGETRY_LOGINDIALOG_H
