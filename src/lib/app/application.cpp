#include "application.h"
#include "application_p.h"

#include <Widgetry/logindialog.h>

#include <Jsoner/object.h>

#include <DataGate/authenticator.h>

using namespace DataGate;

namespace Widgetry {

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
    , d_ptr(new ApplicationPrivate(this))
{
    setApplicationDisplayName("Widgetry App");
    setApplicationName("App");;
    setApplicationVersion(WIDGETRY_VERSION_STR);

    setOrganizationName("Commander Systems");

    setStyle("fusion");

    setWindowIcon(QIcon(":/widgetry/icons/widgetry.png"));

    init(*this);
}

Application::~Application()
{
}

QPixmap Application::splashPixmap() const
{
    QSplashScreen *splash = d_ptr->splash();
    return splash ? splash->pixmap() : QPixmap();
}

void Application::setSplashPixmap(const QPixmap &pixmap)
{
    QSplashScreen *splash = d_ptr->splash();
    splash->setPixmap(pixmap);
    if (!splash->isVisible()) {
        splash->show();
        splash->update();
    }
}

void Application::showSplashMessage(const QString &msg)
{
    showSplashMessage(msg, Qt::AlignHCenter | Qt::AlignBottom, QColor("dark gray"));
}

void Application::showSplashMessage(const QString &msg, int alignment)
{
    showSplashMessage(msg, alignment, QColor("dark gray"));
}

void Application::showSplashMessage(const QString &msg, const QColor &color)
{
    showSplashMessage(msg, Qt::AlignHCenter | Qt::AlignBottom, color);
}

void Application::showSplashMessage(const QString &msg, int alignment, const QColor &color)
{
    d_ptr->splash()->showMessage(msg, alignment, color);
}

void Application::clearSplashMessage()
{
    d_ptr->splash()->clearMessage();
}

void Application::clearSplash(QWidget *widget, bool showWidget)
{
    if (widget) {
        if (showWidget) widget->show();
        d_ptr->splash()->finish(widget);
        return;
    }

    d_ptr->splash()->hide();
}

LoginDialog *Application::loginDialog() const
{
    return d_ptr->loginDialog;
}

void Application::setLoginDialog(LoginDialog *dialog)
{
    d_ptr->loginDialog = dialog;
}

QWidget *Application::mainWidget() const
{
    return d_ptr->mainWidget;
}

void Application::setMainWidget(QWidget *widget)
{
    d_ptr->mainWidget = widget;
}

DataGate::AbstractLoginManager *Application::loginManager() const
{
    return DataGate::Authenticator::loginManager();
}

void Application::setLoginManager(DataGate::AbstractLoginManager *manager)
{
    DataGate::Authenticator::setLoginManager(manager);
}

DataGate::AbstractDataManager *Application::dataManager() const
{
    return d_ptr->dataManager;
}

void Application::setDataManager(DataGate::AbstractDataManager *manager)
{
    d_ptr->dataManager = manager;
}

QObject *Application::object(const char *name) const
{
    return property(name).value<QObject *>();
}

void Application::setObject(const char *name, QObject *instance)
{
    setProperty(name, QVariant::fromValue(instance));
}

QSettings *Application::settings() const
{
    return d_ptr->settings();
}

int Application::exec(LaunchMode mode)
{
    if (mode == ManualLaunch)
        return QApplication::exec();

    QWidget *initialWidget = nullptr;

    if (d_ptr->loginDialog) {
        if (d_ptr->mainWidget)
            connect(d_ptr->loginDialog, &QDialog::accepted, d_ptr->mainWidget, &QWidget::show);

        if (Authenticator::loggedUser().isEmpty()) {
            initialWidget = d_ptr->loginDialog;
            d_ptr->loginDialog->open();
        }
    }

    if (!initialWidget && d_ptr->mainWidget) {
        initialWidget = d_ptr->mainWidget;
        d_ptr->mainWidget->show();
    }

    if (initialWidget && d_ptr->m_splash && d_ptr->m_splash->isVisible())
        clearSplash(initialWidget);

    return QApplication::exec();
}

Application *Application::instance()
{
    return qobject_cast<Application *>(QApplication::instance());
}

ApplicationPrivate::ApplicationPrivate(Application *q)
    : q_ptr(q)
    , loginDialog(nullptr)
    , mainWidget(nullptr)
    , dataManager(nullptr)
    , m_splash(nullptr)
    , m_settings(nullptr)
{

}

ApplicationPrivate::~ApplicationPrivate()
{
    if (m_splash)
        delete m_splash;
}

QSplashScreen *ApplicationPrivate::splash() const
{
    return m_splash;
}

QSplashScreen *ApplicationPrivate::splash()
{
    if (!m_splash)
        m_splash = new QSplashScreen();
    return m_splash;
}

QSettings *ApplicationPrivate::settings()
{
    if (!m_settings)
        m_settings = new QSettings(q_ptr);
    return m_settings;
}

} // namespace Widgetry
