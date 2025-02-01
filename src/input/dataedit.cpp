#include "dataedit.h"
#include "dataedit_p.h"

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qdialogbuttonbox.h>

#include <QtCore/qmetaobject.h>

namespace Widgetry {

class DataEditDialogHelper : public QDialog
{
    Q_OBJECT

public:
    DataEditDialogHelper(QWidget *parent, Qt::WindowFlags flags)
        : QDialog(parent, flags) {}

    void init(DataEdit *edit) {
        QDialog *dialog = this;

        // Create the main layout for the dialog
        QVBoxLayout *layout = new QVBoxLayout(dialog);

        // Add the DataEdit widget (the form to edit the object)
        layout->addWidget(edit);

        m_errorOutput = new QLabel(this);
        m_errorOutput->setWordWrap(true);
        m_errorOutput->setStyleSheet("color: red");
        layout->addWidget(m_errorOutput);

        // Create a button box (Save and Cancel buttons)
        m_buttonBox = new QDialogButtonBox();
        QDialogButtonBox::StandardButtons boxButtons = QDialogButtonBox::Save | QDialogButtonBox::Cancel;
        m_buttonBox->setStandardButtons(boxButtons);
        layout->addWidget(m_buttonBox);

        // Connect edit signals to the appropriate functions
        connect(edit, &DataEdit::complete, this, &DataEditDialogHelper::updateError);
        connect(edit, &DataEdit::complete, this, &DataEditDialogHelper::updateButtons);

        // Connect button signals to the appropriate slots
        connect(m_buttonBox, &QDialogButtonBox::accepted, this, [&] {
            if (m_edit->isComplete())
                accept();
            else
                updateErrorState(true);
        });
        connect(m_buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

        // Set the dialog layout
        dialog->setLayout(layout);

        m_edit = edit;
    }

    void setVisible(bool v) override {
        if (v) {
            updateErrorState(false);
            updateButtonStates(true);
        }
        QDialog::setVisible(v);
    }

    void updateErrorState(bool show) {
        if (show && !m_edit->isComplete()) {
            m_errorOutput->setText(m_edit->completionError());
            m_edit->clearCompletionError();
        } else {
            m_errorOutput->clear();
        }
    }

    void updateButtonStates(bool saveable) {
        if (m_edit->operation() == DataEdit::ShowOperation) {
            m_buttonBox->setStandardButtons(QDialogButtonBox::Close);
        } else {
            QDialogButtonBox::StandardButtons buttons(QDialogButtonBox::Cancel);
            buttons.setFlag(QDialogButtonBox::Save, saveable || m_edit->isComplete());
            m_buttonBox->setStandardButtons(buttons);
        }
    }

public slots:
    void updateError() {
        updateErrorState(true);
    }

    void updateButtons() {
        updateButtonStates(false);
    }

private:
    DataEdit *m_edit;
    QLabel *m_errorOutput;
    QDialogButtonBox *m_buttonBox;
};

DataEdit::DataEdit(QWidget *parent, Qt::WindowFlags flags)
    : DataEdit(new DataEditPrivate(this), parent, flags)
{
}

DataEdit::DataEdit(DataEditPrivate *d, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
    , d_ptr(d)
{
}

DataEdit::~DataEdit()
{
}

Jsoner::Object DataEdit::object() const
{
    extract(d_ptr->object, d_ptr->operation);
    return d_ptr->object;
}

DataEdit::Operation DataEdit::operation() const
{
    return d_ptr->operation;
}

void DataEdit::setObject(const Jsoner::Object &object, Operation operation)
{
    render(object, operation);
    d_ptr->operation = operation;
    d_ptr->object = object;
}

bool DataEdit::isComplete() const
{
    return d_ptr->q_ptr->validateInput();
}

QString DataEdit::completionError() const
{
    return d_ptr->completionErrorString;
}

bool DataEdit::isReadOnly() const
{
    return d_ptr->readOnly;
}

void DataEdit::setReadOnly(bool r)
{
    if (d_ptr->readOnly == r)
        return;

    makeWriteable(!r);
    d_ptr->readOnly = r;
}

QDialog *DataEdit::dialogFromEdit(DataEdit *edit, QWidget *parent, Qt::WindowFlags flags)
{
    // Create the dialog
    DataEditDialogHelper *helper = new DataEditDialogHelper(parent, flags);
    helper->init(edit);
    return helper;
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

bool DataEdit::validateInput()
{
    return true;
}

void DataEdit::setCompletionError(const QString &str)
{
    d_ptr->completionErrorString = str;
}

void DataEdit::clearCompletionError()
{
    d_ptr->completionErrorString.clear();
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

DataEditPrivate::DataEditPrivate(DataEdit *q)
    : q_ptr(q)
    , operation(DataEdit::ShowOperation)
    , readOnly(false)
{
}


} // namespace Widgetry

#include "dataedit.moc"
