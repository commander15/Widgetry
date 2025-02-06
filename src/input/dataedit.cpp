#include "dataedit.h"
#include "dataedit_p.h"

#include <QtCore/qmetaobject.h>

#include "QDateEdit"

namespace Widgetry {

DataEdit::DataEdit(QWidget *parent, Qt::WindowFlags flags)
    : DataEdit(new DataEditPrivate(this), parent, flags)
{
}

DataEdit::DataEdit(DataEditPrivate *d, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
    , AbstractDataEdit(d)
{
    initEditing(this, &DataEdit::editingFinished);
}

DataEdit::~DataEdit()
{
}

QWidget *DataEdit::editWidget() const
{
    return const_cast<DataEdit *>(this);
}

AbstractDataEdit::EditType DataEdit::editType() const
{
    return WidgetEdit;
}

void DataEdit::show()
{
    QWidget::show();
}

void DataEdit::show(const Jsoner::Object &object)
{
    setObject(object, ShowOperation);
    QWidget::show();
}

void DataEdit::add(const Jsoner::Object &object)
{
    setObject(object, AddOperation);
    QWidget::show();
}

void DataEdit::edit(const Jsoner::Object &object)
{
    setObject(object, EditOperation);
    QWidget::show();
}

void DataEdit::clear()
{
    setObject(Jsoner::Object(), d_ptr->operation);
}

bool DataEdit::registerField(QWidget *field)
{
    const QMetaProperty property = field->metaObject()->userProperty();
    if (property.isValid() && property.hasNotifySignal())
        return registerField(field, property.notifySignal().name(), SignalMember);
    else
        return false;
}

bool DataEdit::registerField(QWidget *field, const char *member)
{
    return registerField(field, member, DeduceMember);
}

bool DataEdit::registerField(QWidget *field, const char *member, FieldMemberType type)
{
    const QMetaObject *meta = field->metaObject();

    int signalIndex = -1;

    if (type == PropertyMember) {
        for (int i(0); i < meta->propertyCount(); ++i) {
            const QMetaProperty property = meta->property(i);
            if (strcmp(property.name(), member) == 0) {
                if (property.hasNotifySignal()) {
                    signalIndex = property.notifySignalIndex();
                    break;
                }
            }
        }
    } else if (type == SignalMember) {
        for (int i(0); i < meta->methodCount(); ++i) {
            const QMetaMethod method = meta->method(i);

            if (method.methodType() != QMetaMethod::Signal)
                continue;

            if (strcmp(method.name(), member) == 0) {
                signalIndex = method.methodIndex();
                break;
            }
        }
    } else {
        return registerField(field, member, SignalMember) || registerField(field, member, PropertyMember);
    }

    if (signalIndex >= 0) {
        const QMetaMethod signal = meta->method(signalIndex);

        char handleOverload[33] = "handleFieldChange";
        strncat(handleOverload, "(", 32);
        if (signal.parameterCount() > 0) {
            const QMetaType type(signal.parameterType(0));
            strncat(handleOverload, type.name(), 32);
        }
        strncat(handleOverload, ")", 32);

        const QMetaObject *meta = metaObject();
        for (int i(0); i < meta->methodCount(); ++i) {
            const QMetaMethod method = meta->method(i);
            const QByteArray name = QByteArray::fromRawData(method.name(), strlen(method.name()));

            if (!name.startsWith("handleFieldChange"))
                continue;

            if (QMetaObject::checkConnectArgs(signal, method)) {
                connect(field, signal, this, method);
                return true;
            }
        }
    } else {
        qWarning() << "can't find signal for member" << member << "on" << meta->className();
    }

    return false;
}

void DataEdit::handleFieldChange()
{
    emit complete();
}

void DataEdit::handleFieldChange(bool)
{
    emit complete();
}

void DataEdit::handleFieldChange(int)
{
    emit complete();
}

void DataEdit::handleFieldChange(double)
{
    emit complete();
}

void DataEdit::handleFieldChange(const QString &)
{
    emit complete();
}

void DataEdit::handleFieldChange(const QDate &)
{
    emit complete();
}

void DataEdit::handleFieldChange(const QTime &)
{
    emit complete();
}

void DataEdit::handleFieldChange(const QDateTime &)
{
    emit complete();
}

DataEditPrivate::DataEditPrivate(DataEdit *q)
    : AbstractDataEditPrivate(q)
{
}


} // namespace Widgetry
