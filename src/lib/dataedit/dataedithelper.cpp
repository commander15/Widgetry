#include "dataedithelper_p.h"

#include <QtCore/qmetaobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>

#include <QtWidgets/qwidget.h>

namespace Widgetry {

DataEditHelper::DataEditHelper(QObject *parent)
    : QObject{parent}
{
}

bool DataEditHelper::registerField(QWidget *field, QWidget *edit)
{
    const QMetaProperty property = field->metaObject()->userProperty();
    if (property.isValid() && property.hasNotifySignal())
        return registerField(field, property.notifySignal().name(), edit, SignalMember);
    else
        return false;
}

bool DataEditHelper::registerField(QWidget *field, const char *member, QWidget *edit)
{
    return registerField(field, member, edit, DeduceMember);
}

bool DataEditHelper::registerField(QWidget *field, const char *member, QWidget *edit, FieldMemberType type)
{
    const QMetaObject *meta = field->metaObject();
    int signalIndex = -1;

    if (member == nullptr) {
        if (meta->userProperty().isValid())
            return registerField(field, meta->userProperty().name(), edit, PropertyMember);
        else
            return false;
    }

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
    } else if (meta->indexOfProperty(member) > 0) {
        return registerField(field, member, edit, PropertyMember);
    } else if (meta->indexOfSignal(member) > 0) {
        return registerField(field, member, edit, SignalMember);
    } else {
        return false;
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

        const QMetaObject *meta = this->metaObject();
        for (int i(0); i < meta->methodCount(); ++i) {
            const QMetaMethod method = meta->method(i);
            const QByteArray name = QByteArray::fromRawData(method.name(), strlen(method.name()));

            if (!name.startsWith("handleFieldChange"))
                continue;

            if (QMetaObject::checkConnectArgs(signal, method)) {
                QObject::connect(field, signal, this, method);
                return true;
            }
        }

        field->setProperty("widgetry_edit", QVariant::fromValue(edit));
    } else {
        qWarning() << "can't find signal for member" << member << "on" << meta->className();
    }

    return false;
}

DataEditHelper *DataEditHelper::global()
{
    static DataEditHelper helper;
    return &helper;
}

void DataEditHelper::handleFieldChange()
{
    emitCompleteChanged(sender());
}

void DataEditHelper::handleFieldChange(bool)
{
    emitCompleteChanged(sender());
}

void DataEditHelper::handleFieldChange(int)
{
    emitCompleteChanged(sender());
}

void DataEditHelper::handleFieldChange(double)
{
    emitCompleteChanged(sender());
}

void DataEditHelper::handleFieldChange(const QString &)
{
    emitCompleteChanged(sender());
}

void DataEditHelper::handleFieldChange(const QDate &)
{
    emitCompleteChanged(sender());
}

void DataEditHelper::handleFieldChange(const QTime &)
{
    emitCompleteChanged(sender());
}

void DataEditHelper::handleFieldChange(const QDateTime &)
{
    emitCompleteChanged(sender());
}

void DataEditHelper::emitCompleteChanged(QObject *field)
{
    QWidget *edit = field->property("widgetry_edit").value<QWidget *>();
    QMetaObject::invokeMethod(edit, "completeChanged");
}

} // namespace Widgetry
