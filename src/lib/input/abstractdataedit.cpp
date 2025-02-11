#include "abstractdataedit.h"
#include "abstractdataedit_p.h"

#include <Widgetry/private/dataeditdialog_p.h>

namespace Widgetry {

AbstractDataEdit::AbstractDataEdit()
    : d_ptr(new AbstractDataEditPrivate(this))
{
}

AbstractDataEdit::AbstractDataEdit(AbstractDataEditPrivate *d)
    : d_ptr(d)
{
}

AbstractDataEdit::~AbstractDataEdit() = default;

Jsoner::Object AbstractDataEdit::object() const
{
    extract(d_ptr->object, d_ptr->operation);
    return d_ptr->object;
}

AbstractDataEdit::Operation AbstractDataEdit::operation() const
{
    return d_ptr->operation;
}

void AbstractDataEdit::setObject(const Jsoner::Object &object, Operation operation)
{
    render(object, operation);
    d_ptr->object = object;
    d_ptr->operation = operation;

    setReadOnly(operation == ShowOperation);
}

bool AbstractDataEdit::isComplete() const
{
    AbstractDataEdit *edit = const_cast<AbstractDataEdit *>(this);
    edit->clearCompletionError();
    return edit->validateInput();
}

QString AbstractDataEdit::completionError() const
{
    return d_ptr->completionErrorString;
}

bool AbstractDataEdit::isReadOnly() const
{
    return d_ptr->readOnly;
}

void AbstractDataEdit::setReadOnly(bool r)
{
    if (d_ptr->readOnly == r)
        return;

    makeWriteable(!r);
    d_ptr->readOnly = r;
}

DataInterface *AbstractDataEdit::interface() const
{
    return d_ptr->interface;
}

void AbstractDataEdit::setInterface(DataInterface *interface)
{
    d_ptr->interface = interface;
}

QWidget *AbstractDataEdit::editWidget() const
{
    AbstractDataEdit *edit = const_cast<AbstractDataEdit *>(this);
    return reinterpret_cast<QWidget *>(edit);
}

QDialog *AbstractDataEdit::dialogFromEdit(AbstractDataEdit *edit, QWidget *parent, Qt::WindowFlags flags)
{
    DataEditDialogHelper *dialog = new DataEditDialogHelper(parent, flags);
    dialog->init(edit);
    return dialog;
}

AbstractDataEdit *AbstractDataEdit::editFromDialog(QDialog *dialog)
{
    return qobject_cast<DataEditDialogHelper *>(dialog);
}

void AbstractDataEdit::show()
{
    QWidget *w = editWidget();

    if (w->inherits("QDialog")) {
        QDialog *d = static_cast<QDialog *>(w);
        d->open();
        return;
    }

    if (!w->isVisible())
        w->show();
    else if (!w->hasFocus())
        w->setFocus();
    else if (!w->isActiveWindow())
        w->activateWindow();
}

void AbstractDataEdit::show(const Jsoner::Object &object)
{
    setObject(object, ShowOperation);
    show();
}

void AbstractDataEdit::add(const Jsoner::Object &object)
{
    setObject(object, AddOperation);
    show();
}

void AbstractDataEdit::edit(const Jsoner::Object &object)
{
    setObject(object, EditOperation);
    show();
}

void AbstractDataEdit::reset()
{
    render(d_ptr->object, d_ptr->operation);
}

void AbstractDataEdit::clear()
{
    setObject(Jsoner::Object(), d_ptr->operation);
}

void AbstractDataEdit::exec(const DataEditFinishedCallback &onFinished)
{
    d_ptr->finishCallback = onFinished;

    QWidget *w = editWidget();

    if (w->inherits("QDialog"))
        static_cast<QDialog *>(w)->open();
    else
        show();
}

AbstractDataEdit::EditType AbstractDataEdit::editType() const
{
    return CustomEdit;
}

void AbstractDataEdit::setCompletionError(const QString &str)
{
    d_ptr->completionErrorString = str;
}

void AbstractDataEdit::clearCompletionError()
{
    d_ptr->completionErrorString.clear();
}

void AbstractDataEdit::finishEditing(int result)
{
    if (d_ptr->finishCallback) {
        d_ptr->finishCallback(object(), result);

        if (d_ptr->operation != EditOperation)
            d_ptr->finishCallback = nullptr;
    }
}

void AbstractDataEdit::handleResponse(const DataResponse &response)
{
    Q_UNUSED(response);
}

AbstractDataEditPrivate::AbstractDataEditPrivate(AbstractDataEdit *q)
    : q_ptr(q)
    , operation(AbstractDataEdit::ShowOperation)
    , readOnly(false)
    , finishCallback(nullptr)
    , interface(nullptr)
{
}

} // namespace Widgetry
