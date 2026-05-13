#include "dialog.h"
#include "dialog_p.h"

namespace Widgetry {

Dialog::Dialog(const QByteArray &id, QWidget *parent, Qt::WindowFlags flags)
    : Dialog(new DialogPrivate(this, id), parent, flags)
{
}

Dialog::Dialog(QWidget *parent, Qt::WindowFlags flags)
    : Dialog(new DialogPrivate(this, QByteArray()), parent, flags)
{
}

Dialog::Dialog(DialogPrivate *d, QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
    , AbstractWidget(d)
{
    d->initWidget<Dialog>();
}

void Dialog::processIconChange(const QIcon &icon)
{
    emit iconChanged(icon);
}

void Dialog::processTitleChange(const QString &title)
{
    emit titleChanged(title);
}

void Dialog::processActionChange(QAction *current, QAction *previous)
{
    if (previous) {
        disconnect(this, &Dialog::iconChanged, previous, &QAction::setIcon);
        disconnect(this, &Dialog::titleChanged, previous, &QAction::setText);
    }

    if (current) {
        setIcon(current->icon());
        setTitle(current->text());
        connect(this, &Dialog::iconChanged, current, &QAction::setIcon);
        connect(this, &Dialog::titleChanged, current, &QAction::setText);
    }
}

void Dialog::processOperationSupportChanged(const QString &operation, bool supported)
{
    emit operationSupportChanged(operation, supported);
}

bool Dialog::event(QEvent *event)
{
    d_ptr->processEvent(event);
    return QDialog::event(event);
}

Dialog::~Dialog()
{
}

void Dialog::sync()
{
    AbstractWidget::sync();
}

DialogPrivate::DialogPrivate(Dialog *q, const QByteArray &id)
    : AbstractWidgetPrivate(q, q)
{
    this->id = id;
}

} // namespace Widgetry
