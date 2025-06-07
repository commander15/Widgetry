#include "application.h"
#include "application_p.h"

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
    } else {
        d_ptr->splash()->hide();
    }
}

QSettings *Application::settings() const
{
    return d_ptr->settings();
}

Application *Application::instance()
{
    return qobject_cast<Application *>(QApplication::instance());
}

ApplicationPrivate::ApplicationPrivate(Application *q)
    : q_ptr(q)
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
