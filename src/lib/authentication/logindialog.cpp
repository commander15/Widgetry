#include "logindialog.h"
#include "ui_logindialog.h"

#include <DataGate/authenticator.h>

#include <QtGui/qvalidator.h>

#include <QtCore/qtimer.h>
#include <QtCore/qregularexpression.h>

using namespace DataGate;

namespace Widgetry {

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    m_showOnLogOut(false),
    m_cleanTimer(new QTimer(this))
{
    ui->setupUi(this);

    ui->identifierInput->addAction(ui->actionLogin, QLineEdit::LeadingPosition);

    ui->passwordInput->addAction(ui->actionPassword, QLineEdit::LeadingPosition);
    ui->passwordInput->addAction(ui->actionShowPassword, QLineEdit::TrailingPosition);
    connect(ui->actionShowPassword, &QAction::triggered, this, &LoginDialog::togglePasswordVisibility);

    connect(ui->logInButton, &QAbstractButton::clicked, this, &LoginDialog::logIn);

    m_cleanTimer->setInterval(3000);
    m_cleanTimer->setSingleShot(true);
    connect(m_cleanTimer, &QTimer::timeout, this, &LoginDialog::hideError);

    Authenticator *auth = Authenticator::global();
    connect(auth, &Authenticator::loggedIn, this, [this](const Jsoner::Object &user) { if (isVisible()) accept(); });
    connect(auth, &Authenticator::logInFailed, this, &LoginDialog::showError);

    connect(this, &QDialog::rejected, qApp, &QCoreApplication::quit);

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
    QVariant currentType = ui->identifierInput->property("type");
    return (currentType.isValid() ? static_cast<IdentifierType>(currentType.toInt()) : UnknownIdentifier);
}

void LoginDialog::setIdentifierType(IdentifierType type)
{
    static const QList<QRegularExpression> expressions = {
        QRegularExpression("^[a-zA-Z0-9_]{3,16}$"),
        QRegularExpression("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$")
    };

    switch (type) {
    case LoginIdentifier:
        ui->identifierInput->setPlaceholderText(tr("Login"));
        break;

    case EmailIdentifier:
        ui->identifierInput->setPlaceholderText(tr("Email"));
        break;

    default:
        ui->identifierInput->setPlaceholderText(tr("Identifier"));
        break;
    }

    if (type == UnknownIdentifier) {
        const QValidator *validator = identifierValidator();
        if (validator && validator->property("internal").toBool()) {
            setIdentifierValidator(nullptr);
            delete const_cast<QValidator *>(validator);
        }
        ui->identifierInput->setProperty("type", QVariant());
        return;
    }

    QRegularExpressionValidator *validator = new QRegularExpressionValidator(expressions.at(type), this);
    validator->setProperty("internal", true);
    setIdentifierValidator(validator);

    ui->identifierInput->setProperty("type", type);
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
    QDialog::setVisible(visible);

    if (visible)
        restore(true);
}

void LoginDialog::togglePasswordVisibility()
{
    bool hidden = (ui->passwordInput->echoMode() == QLineEdit::Password);
    if (hidden) {
        ui->passwordInput->setEchoMode(QLineEdit::Normal);
        ui->actionShowPassword->setIcon(QIcon(":/widgetry/icons/password_hide.png"));
    } else {
        ui->passwordInput->setEchoMode(QLineEdit::Password);
        ui->actionShowPassword->setIcon(QIcon(":/widgetry/icons/password_show.png"));
    }
}

void LoginDialog::logIn()
{
    if (m_cleanTimer->isActive())
        m_cleanTimer->stop();

    ui->identifierInput->setReadOnly(true);
    ui->identifierInput->clearFocus();

    ui->passwordInput->setReadOnly(true);
    ui->passwordInput->clearFocus();

    ui->logInButton->setEnabled(false);

    Authenticator::logIn(ui->identifierInput->text(), ui->passwordInput->text());
    QTimer::singleShot(300, this, [this] {
        if (ui->stackedWidget->currentIndex() == 0)
            ui->stackedWidget->setCurrentIndex(1);
    });
}

void LoginDialog::showError(const AuthenticationError &error)
{
    ui->errorOutput->setText(error.errorString());
    ui->stackedWidget->setCurrentIndex(2);

    restore(false);
    m_cleanTimer->start();
}

void LoginDialog::hideError()
{
    restore(true);
}

void LoginDialog::restore(bool full)
{
    ui->identifierInput->setReadOnly(false);

    ui->passwordInput->setReadOnly(false);
    ui->passwordInput->clear();

    if (full)
        ui->stackedWidget->setCurrentIndex(0);

    if (ui->identifierInput->text().isEmpty())
        ui->identifierInput->setFocus();
    else
        ui->passwordInput->setFocus();

    ui->logInButton->setEnabled(true);
}

}
