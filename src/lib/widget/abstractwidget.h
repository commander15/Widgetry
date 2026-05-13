#ifndef WIDGETRY_ABSTRACTWIDGET_H
#define WIDGETRY_ABSTRACTWIDGET_H

#include <Widgetry/global.h>

#include <QtCore/qscopedpointer.h>

#include <QtWidgets/qwidget.h>

class QSettings;

namespace Widgetry {

class WidgetRequest;
class WidgetResponse;

class AbstractWidgetPrivate;
class WIDGETRY_EXPORT AbstractWidget
{
public:
    virtual ~AbstractWidget();

    QByteArray id() const;
    void setId(const QByteArray &id);

    QIcon icon() const;
    void setIcon(const QIcon &icon);

    QString title() const;
    void setTitle(const QString &title);

    QAction *action() const;
    void setAction(QAction *action);

    virtual bool isOperationSupported(const QString &operation) const;
    QStringList supportedOperations() const;
    virtual QStringList availableOperations() const;

    QVariant operate(const QString &operation, bool *success = nullptr);
    QVariant operate(const QString &operation, const QVariant &parameter, bool *success = nullptr);
    QVariant operate(const QString &operation, const QVariantMap &parameters, bool *success = nullptr);

    virtual void sync();

    virtual void loadSettings(QSettings *settings);
    virtual void saveSettings(QSettings *settings) const;

    QWidget *widget() const;
    QDialog *dialog() const;

protected:
    AbstractWidget(AbstractWidgetPrivate *d);

    virtual void processIdChange(const QByteArray &id);
    virtual void processIconChange(const QIcon &icon) = 0;
    virtual void processTitleChange(const QString &title) = 0;
    virtual void processActionChange(QAction *current, QAction *previous) = 0;

    virtual void processOperationSupportChanged(const QString &operation, bool supported) = 0;

    virtual void prepareUi(bool firstShow = false);
    virtual void cleanupUi();
    virtual void translateUi(bool full = true);

    void requestOperation(const QString &name, const QByteArray &target);
    void requestOperation(WidgetRequest &request);
    virtual void processRequestDispatch(const WidgetRequest &request);

    virtual WidgetResponse handleRequest(const WidgetRequest &request);
    virtual void handleResponse(const WidgetRequest &request, const WidgetResponse &response);

    QScopedPointer<AbstractWidgetPrivate> d_ptr;

private:
    Q_DISABLE_COPY_MOVE(AbstractWidget)

    friend class AbstractWidgetPrivate;
};

} // namespace Widgetry

#endif // WIDGETRY_ABSTRACTWIDGET_H
