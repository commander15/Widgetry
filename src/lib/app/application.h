#ifndef WIDGETRY_APPLICATION_H
#define WIDGETRY_APPLICATION_H

#include <Widgetry/global.h>

#include <QtWidgets/qapplication.h>

class QSettings;

namespace Widgetry {

class LoginDialog;

class ApplicationPrivate;
class WIDGETRY_EXPORT Application : public QApplication
{
    Q_OBJECT

public:
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

    QSettings *settings() const;

    static Application *instance();

private:
    QScopedPointer<ApplicationPrivate> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_APPLICATION_H
