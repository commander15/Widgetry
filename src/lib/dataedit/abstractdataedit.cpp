#include "abstractdataedit.h"
#include "abstractdataedit_p.h"

#include <Widgetry/private/dataedithelper_p.h>

#include <QtCore/qmetaobject.h>

#include <QtWidgets/qwidget.h>

namespace Widgetry {

AbstractDataEdit::AbstractDataEdit(QWidget *widget)
    : d_ptr(new AbstractDataEditPrivate(this, widget))
{
}

AbstractDataEdit::~AbstractDataEdit() = default;

AbstractDataEdit::Operation AbstractDataEdit::operation() const
{
    WIDGETRY_D(const AbstractDataEdit);
    return d->operation;
}Jsoner::Object AbstractDataEdit::object() const
{
    WIDGETRY_D(AbstractDataEdit);
    extract(d->object);
    return d->object;
}

void AbstractDataEdit::setObject(const Jsoner::Object &object)
{
    WIDGETRY_D(AbstractDataEdit);
    d->object = object;
    render(object);
}

void AbstractDataEdit::setObject(const Jsoner::Object &object, Operation operation)
{
    WIDGETRY_D(AbstractDataEdit);
    d->object = object;
    d->operation = operation;
    render(object);

    setReadOnly(operation == ShowOperation);
}

void AbstractDataEdit::reset()
{
    WIDGETRY_D(AbstractDataEdit);
    render(d->object);
}

void AbstractDataEdit::clear()
{
    WIDGETRY_D(AbstractDataEdit);
    d_ptr->object = Jsoner::Object();
    render(d_ptr->object);
}

bool AbstractDataEdit::hasValidInput() const
{
    AbstractDataEdit *edit = const_cast<AbstractDataEdit *>(this);
    edit->clearValidationError();
    return edit->validateInput();
}

QString AbstractDataEdit::validationError() const
{
    WIDGETRY_D(const AbstractDataEdit);
    return d->validationErrorString;
}

bool AbstractDataEdit::isReadOnly() const
{
    WIDGETRY_D(const AbstractDataEdit);
    return d->readOnly;
}

void AbstractDataEdit::setReadOnly(bool r)
{
    WIDGETRY_D(AbstractDataEdit);
    if (d->readOnly == r)
        return;

    makeWriteable(!r);
    d->readOnly = r;
}

void AbstractDataEdit::begin()
{
    makeVisible(true);
}

void AbstractDataEdit::end(int result)
{
    WIDGETRY_D(AbstractDataEdit);
    if (d->finishCallback) {
        d->finishCallback(object(), result);
        d->finishCallback = nullptr;
    }

    makeVisible(false, result);
}

void AbstractDataEdit::run(const DataEditFinishedCallback &onFinished)
{
    WIDGETRY_D(AbstractDataEdit);
    d->finishCallback = onFinished;
    begin();
}

DataBrowser *AbstractDataEdit::browser() const
{
    WIDGETRY_D(const AbstractDataEdit);
    return d->browser;
}

void AbstractDataEdit::setBrowser(DataBrowser *browser)
{
    WIDGETRY_D(AbstractDataEdit);
    d->browser = browser;
}

QWidget *AbstractDataEdit::editWidget() const
{
    return d_ptr->widget;
}

QString AbstractDataEdit::fieldName(QWidget *widget)
{
    QString name = widget->property("fieldName").toString();
    if (!name.isEmpty())
        return name;
    else
        return widget->objectName();
}

void AbstractDataEdit::setFieldName(QWidget *widget, const QString &name)
{
    widget->setProperty("fieldName", name);
}

QVariant AbstractDataEdit::fieldValue(QWidget *widget)
{
    const QMetaObject *meta = widget->metaObject();
    QMetaProperty user = meta->userProperty();

    if (!user.isValid() || !user.isReadable())
        return QVariant();

    return user.read(widget);
}

bool AbstractDataEdit::setFieldValue(QWidget *widget, const QVariant &value)
{
    const QMetaObject *meta = widget->metaObject();
    QMetaProperty user = meta->userProperty();

    if (!user.isValid() || !user.isWritable())
        return false;

    return user.write(widget, value);
}

bool AbstractDataEdit::clearFieldValue(QWidget *widget)
{
    const QMetaObject *meta = widget->metaObject();

    // First, we try to find a clear method
    int methodIndex = meta->indexOfMethod("clear");
    if (methodIndex >= 0) {
        QMetaMethod method = meta->method(methodIndex);
        return method.invoke(widget);
    }

    // No clear method, going from the user property
    QMetaProperty user = meta->userProperty();

    if (!user.isValid())
        return false;

    if (user.isResettable())
        return user.reset(widget);

    if (user.isWritable())
        return user.write(widget, QVariant(user.metaType()));

    return false;
}

bool AbstractDataEdit::makeFieldWriteable(QWidget *widget, bool writeable)
{
    const QMetaObject *meta = widget->metaObject();

    // First, we try to find a 'readOnly' property
    int readOnlyIndex = meta->indexOfProperty("readOnly");
    if (readOnlyIndex >= 0) {
        QMetaProperty readOnly = meta->property(readOnlyIndex);
        if (readOnly.isWritable())
            return readOnly.write(widget, writeable);
    }

    // Next, we try to find a 'setReadOnly' method
    int methodIndex = meta->indexOfMethod("setReadOnly");
    if (methodIndex >= 0) {
        QMetaMethod method = meta->method(methodIndex);
        return method.invoke(widget, writeable);
    }

    // As last ressort, we just use enabled state of the widget
    widget->setEnabled(writeable);
    return true;
}

bool AbstractDataEdit::registerField(QWidget *field)
{
    return DataEditHelper::global()->registerField(field, nullptr, d_ptr->widget, DataEditHelper::DeduceMember);
}

bool AbstractDataEdit::registerField(QWidget *field, const char *member)
{
    return DataEditHelper::global()->registerField(field, member, d_ptr->widget, DataEditHelper::DeduceMember);
}

bool AbstractDataEdit::registerField(QWidget *field, const char *member, FieldMemberType type)
{
    return DataEditHelper::global()->registerField(field, member, d_ptr->widget, static_cast<DataEditHelper::FieldMemberType>(type));
}

void AbstractDataEdit::setValidationError(const QString &str)
{
    WIDGETRY_D(AbstractDataEdit);
    d->validationErrorString = str;
}

void AbstractDataEdit::clearValidationError()
{
    WIDGETRY_D(AbstractDataEdit);
    d->validationErrorString.clear();
}

bool AbstractDataEdit::validateInput()
{
    return true;
}

void AbstractDataEdit::makeWriteable(bool writable)
{
    d_ptr->widget->setEnabled(writable);
}

void AbstractDataEdit::makeVisible(bool visibible, int result)
{
    QWidget *w = d_ptr->widget;

    if (visibible && !w->isVisible())
        d_ptr->widget->show();

    if (!visibible && w->isVisible())
        d_ptr->widget->hide();

    Q_UNUSED(result);
}

AbstractDataEditPrivate::AbstractDataEditPrivate(AbstractDataEdit *q, QWidget *widget)
    : q_ptr(q)
    , widget(widget)
    , operation(AbstractDataEdit::ShowOperation)
    , readOnly(false)
    , finishCallback(nullptr)
    , browser(nullptr)
{
}

} // namespace Widgetry
