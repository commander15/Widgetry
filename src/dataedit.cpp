#include "dataedit.h"
#include "dataedit_p.h"

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qdialogbuttonbox.h>

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
        m_errorOutput->setStyleSheet("color: red");
        layout->addWidget(m_errorOutput);

        // Create a button box (Save and Cancel buttons)
        m_buttonBox = new QDialogButtonBox();
        QDialogButtonBox::StandardButtons boxButtons = QDialogButtonBox::Save | QDialogButtonBox::Cancel;
        m_buttonBox->setStandardButtons(boxButtons);
        layout->addWidget(m_buttonBox);

        // Connect edit signals to the appropriate functions
        connect(edit, &DataEdit::completeChanged, this, &DataEditDialogHelper::updateError);
        connect(edit, &DataEdit::completeChanged, this, &DataEditDialogHelper::updateButtons);

        // Connect button signals to the appropriate slots
        connect(m_buttonBox, &QDialogButtonBox::accepted, this, [&] {
            if (m_edit->isComplete())
                accept();
            else
                updateError();
        });
        connect(m_buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

        // Set the dialog layout
        dialog->setLayout(layout);

        m_edit = edit;
    }

    void setVisible(bool v) override {
        updateError(true);
        updateButtons(true);
        QDialog::setVisible(v);
    }

public slots:
    void updateError(bool hide = false) {
        if (m_edit->isComplete())
            return;

        const QString str = m_edit->completionError();
        if (!str.isEmpty())
            m_errorOutput->setText(str);
        m_errorOutput->setVisible(!hide || !str.isEmpty());
    }

    void updateButtons(bool ignore = false) {
        if (m_edit->operation() == DataEdit::ShowOperation) {
            m_buttonBox->setStandardButtons(QDialogButtonBox::Close);
        } else {
            QDialogButtonBox::StandardButtons buttons(QDialogButtonBox::Cancel);
            buttons.setFlag(QDialogButtonBox::Save, ignore || m_edit->isComplete());
            m_buttonBox->setStandardButtons(buttons);
        }
    }

private:
    DataEdit *m_edit;
    QLabel *m_errorOutput;
    QDialogButtonBox *m_buttonBox;
};

DataEdit::DataEdit(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
    , d_ptr(new DataEditPrivate(this))
{
}

GenO::Object DataEdit::object() const
{
    extract(d_ptr->object, d_ptr->operation);
    return d_ptr->object;
}

void DataEdit::setObject(const GenO::Object &object, Operation operation)
{
    render(object, operation);
    d_ptr->operation = operation;
    d_ptr->object = object;
}

bool DataEdit::isComplete() const
{
    return true;
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

void DataEdit::show(const GenO::Object &object)
{
    setObject(object, ShowOperation);
    QWidget::show();
}

void DataEdit::add(const GenO::Object &object)
{
    setObject(object, AddOperation);
    QWidget::show();
}

void DataEdit::edit(const GenO::Object &object)
{
    setObject(object, EditOperation);
    QWidget::show();
}

void DataEdit::clear()
{
    setObject(GenO::Object(), d_ptr->operation);
}

void DataEdit::setCompletionErrorString(const QString &str)
{
    d_ptr->completionErrorString = str;
}

DataEditPrivate::DataEditPrivate(DataEdit *q)
    : q_ptr(q)
    , operation(DataEdit::ShowOperation)
    , readOnly(false)
{
}


} // namespace Widgetry

#include "dataedit.moc"
