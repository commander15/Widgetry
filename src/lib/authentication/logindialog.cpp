#include "logindialog.h"
#include "ui_logindialog.h"

#include <Jsoner/object.h>

#include <DataGate/authenticator.h>

#include <QtCore/qtimer.h>
#include <QtCore/qsettings.h>

using namespace DataGate;

namespace Widgetry {

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    m_showOnLogOut(false),
    m_cleanTimer(new QTimer(this))
{
    ui->setupUi(this);

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &LoginDialog::adaptToView);
    connect(ui->logInButton, &QAbstractButton::clicked, this, &LoginDialog::logIn);

    Authenticator *auth = Authenticator::global();
    connect(auth, &Authenticator::loggedIn, this, &LoginDialog::showMessage);
    connect(auth, &Authenticator::logInFailed, this, &LoginDialog::showError);

    m_cleanTimer->setInterval(2500);
    m_cleanTimer->setSingleShot(true);
    connect(m_cleanTimer, &QTimer::timeout, this, &LoginDialog::hideError);

    setLogo(QPixmap(":/widgetry/icons/widgetry.png"));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QPixmap LoginDialog::logo() const
{
    return ui->logoOutput->pixmap();
}

QString LoginDialog::identifier() const
{
    return ui->identifierInput->text();
}

QString LoginDialog::password() const
{
    return ui->passwordInput->text();
}

LoginDialog::IdentifierType LoginDialog::identifierType() const
{
    return static_cast<IdentifierType>(ui->identifierInput->loginType());
}

void LoginDialog::setIdentifierType(IdentifierType type)
{
    ui->identifierInput->setLoginType(static_cast<LoginEdit::LoginType>(type));
}

const QValidator *LoginDialog::identifierValidator() const
{
    return ui->identifierInput->validator();
}

void LoginDialog::setIdentifierValidator(const QValidator *validator)
{
    ui->identifierInput->setValidator(validator);
}

const QValidator *LoginDialog::passwordValidator() const
{
    return ui->passwordInput->validator();
}

void LoginDialog::setPasswordValidator(const QValidator *validator)
{
    ui->passwordInput->setValidator(validator);
}

void LoginDialog::showOnLogOut()
{
    setShowOnLogOut(true);
}

void LoginDialog::setShowOnLogOut(bool show)
{
    if (m_showOnLogOut == show)
        return;

    Authenticator *auth = Authenticator::global();
    if (show)
        connect(auth, &Authenticator::loggedOut, this, &QDialog::open);
    else
        disconnect(auth, &Authenticator::loggedOut, this, &QDialog::open);

    m_showOnLogOut = show;
}

void LoginDialog::loadSettings(QSettings *settings)
{
    settings->beginGroup("login");
    settings->setValue(ui->identifierInput->placeholderText(), ui->identifierInput->text());
    settings->endGroup();
}

void LoginDialog::saveSettings(QSettings *settings) const
{
    settings->beginGroup("login");
    ui->identifierInput->setText(settings->value(ui->identifierInput->placeholderText()).toString());
    settings->endGroup();
}

void LoginDialog::setLogo(const QPixmap &logo)
{
    ui->logoOutput->setPixmap(logo.scaled(ui->logoOutput->size(), Qt::KeepAspectRatio));
}

void LoginDialog::setIdentifier(const QString &identifier)
{
    ui->identifierInput->setText(identifier);
}

void LoginDialog::setPassword(const QString &password)
{
    ui->passwordInput->setText(password);
}

void LoginDialog::setVisible(bool visible)
{
    if (visible)
        ui->stackedWidget->setCurrentIndex(0);
    QDialog::setVisible(visible);
}

void LoginDialog::logIn()
{
    if (m_cleanTimer->isActive())
        m_cleanTimer->stop();

    ui->stackedWidget->setCurrentIndex(1);

    Authenticator::logIn(ui->identifierInput->text(), ui->passwordInput->text());
}

void LoginDialog::showMessage(const Jsoner::Object &user)
{
    Q_UNUSED(user);
    accept();
}

void LoginDialog::showError(const AuthenticationError &error)
{
    ui->errorOutput->setText(error.text());
    ui->stackedWidget->setCurrentIndex(2);
    m_cleanTimer->start();
}

void LoginDialog::hideError()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void LoginDialog::adaptToView(int index)
{
    const bool main = (index == 0);

    ui->identifierInput->setReadOnly(!main);
    ui->passwordInput->setReadOnly(!main);

    if (main) {
        if (ui->identifierInput->text().isEmpty())
            ui->identifierInput->setFocus();
        else
            ui->passwordInput->setFocus();
        ui->passwordInput->clear();
    }
}

}
