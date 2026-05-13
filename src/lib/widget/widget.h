#ifndef WIDGETRY_WIDGET_H
#define WIDGETRY_WIDGET_H

#include <Widgetry/global.h>
#include <Widgetry/abstractwidget.h>

#include <QtWidgets/qwidget.h>

class QSettings;

namespace Widgetry {

class WidgetPrivate;
class WIDGETRY_EXPORT Widget : public QWidget, public AbstractWidget
{
    Q_OBJECT

public:
    Widget(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    Widget(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~Widget();

public slots:
    void sync() override;

signals:
    void iconChanged(const QIcon &icon);
    void titleChanged(const QString &title);
    void operationSupportChanged(const QString &operation, bool supported);

protected:
    Widget(WidgetPrivate *d, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    void processIconChange(const QIcon &icon) override;
    void processTitleChange(const QString &title) override;
    void processActionChange(QAction *current, QAction *previous) override;
    void processOperationSupportChanged(const QString &operation, bool supported) override;

    bool event(QEvent *event) override;

    using AbstractWidget::d_ptr;

private:
    Q_DISABLE_COPY_MOVE(Widget)
};

} // namespace Widgetry

#endif // WIDGETRY_WIDGET_H
