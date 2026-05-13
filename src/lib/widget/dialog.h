#ifndef WIDGETRY_DIALOG_H
#define WIDGETRY_DIALOG_H

#include <Widgetry/global.h>
#include <Widgetry/abstractwidget.h>

#include <QtWidgets/qdialog.h>

namespace Widgetry {

class DialogPrivate;
class WIDGETRY_EXPORT Dialog : public QDialog, public AbstractWidget
{
    Q_OBJECT

public:
    Dialog(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    Dialog(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~Dialog();

public slots:
    void sync() override;

signals:
    void iconChanged(const QIcon &icon);
    void titleChanged(const QString &title);
    void operationSupportChanged(const QString &operation, bool supported);

protected:
    Dialog(DialogPrivate *d, QWidget *parent, Qt::WindowFlags flags);

    void processIconChange(const QIcon &icon) override;
    void processTitleChange(const QString &title) override;
    void processActionChange(QAction *current, QAction *previous) override;
    void processOperationSupportChanged(const QString &operation, bool supported) override;

    bool event(QEvent *event) override;

    using AbstractWidget::d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DIALOG_H
