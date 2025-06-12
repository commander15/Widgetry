#ifndef WIDGETRY_APPLICATION_P_H
#define WIDGETRY_APPLICATION_P_H

#include "application.h"

#include <QtCore/qsettings.h>

#include <QtWidgets/qsplashscreen.h>

namespace Widgetry
{

class ApplicationPrivate : public QObject
{
    Q_OBJECT

public:
    ApplicationPrivate(Application *q);
    ~ApplicationPrivate();

    QSplashScreen *splash() const;
    QSplashScreen *splash();

    Application *q_ptr;

    LoginDialog *loginDialog;
    QWidget *mainWidget;

    //DataGate::AbstractLoginManager *loginManager;
    DataGate::AbstractDataManager *dataManager;

    QSettings *settings();

private:
    QSplashScreen *m_splash;
    QSettings *m_settings;

    friend class Application;
};

}

#endif // WIDGETRY_APPLICATION_P_H
