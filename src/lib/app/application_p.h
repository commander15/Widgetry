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

    QSettings *settings();

    Application *q_ptr;

private:
    QSplashScreen *m_splash;
    QSettings *m_settings;
};

}

#endif // WIDGETRY_APPLICATION_P_H
