#ifndef WIDGETRY_APPLICATION_H
#define WIDGETRY_APPLICATION_H

#include <Widgetry/global.h>

#include <QtWidgets/qapplication.h>

class QSettings;

namespace DataGate {
class AbstractLoginManager;
class AbstractDataManager;
}

namespace Widgetry {

class LoginDialog;

class ApplicationPrivate;
class WIDGETRY_EXPORT Application : public QApplication
{
    Q_OBJECT

public:
    enum LaunchMode {
        AutoLauch,
        ManualLaunch
    };

    explicit Application(int &argc, char **argv);
    virtual ~Application();

    QPixmap splashPixmap() const;
    void setSplashPixmap(const QPixmap &pixmap);

    Q_SLOT void showSplashMessage(const QString &msg);
    Q_SLOT void showSplashMessage(const QString &msg, int alignment);
    Q_SLOT void showSplashMessage(const QString &msg, const QColor &color);
    Q_SLOT void showSplashMessage(const QString &msg, int alignment, const QColor &color);
    Q_SLOT void clearSplashMessage();
    void clearSplash(QWidget *widget = nullptr, bool showWidget = false);

    LoginDialog *loginDialog() const;
    void setLoginDialog(LoginDialog *dialog);

    QWidget *mainWidget() const;
    void setMainWidget(QWidget *widget);

    DataGate::AbstractLoginManager *loginManager() const;
    void setLoginManager(DataGate::AbstractLoginManager *manager);

    DataGate::AbstractDataManager *dataManager() const;
    void setDataManager(DataGate::AbstractDataManager *manager);

    QSettings *settings() const;

    int exec(LaunchMode mode);

    static Application *instance();

private:
    QScopedPointer<ApplicationPrivate> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_APPLICATION_H
