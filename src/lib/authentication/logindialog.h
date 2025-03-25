#ifndef WIDGETRY_LOGINDIALOG_H
#define WIDGETRY_LOGINDIALOG_H

#include <Widgetry/global.h>

#include <QtWidgets/qdialog.h>

namespace DataGate {
class AuthenticationError;
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
        LoginIdentifier,
        EmailIdentifier,

        UnknownIdentifier = -1
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
    void togglePasswordVisibility();

    void logIn();
    void showError(const DataGate::AuthenticationError &error);
    void hideError();

private:
    void restore(bool full);

    Ui::LoginDialog *ui;

    bool m_showOnLogOut;
    QTimer *m_cleanTimer;
};

}

#endif // WIDGETRY_LOGINDIALOG_H
