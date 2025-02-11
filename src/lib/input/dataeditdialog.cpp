#include "dataeditdialog_p.h"

#include <Widgetry/dataedit.h>

#include <Widgetry/private/abstractdataedit_p.h>

#include <QtWidgets/qboxlayout.h>

namespace Widgetry {

DataEditDialogHelper::DataEditDialogHelper(QWidget *parent, Qt::WindowFlags flags, bool multiple)
    : QDialog(parent, flags | (multiple ? Qt::Window : Qt::WindowFlags()))
    , m_multiple(multiple)
{
    initEditing(this, &QDialog::finished);

    setWindowModality(multiple ? Qt::NonModal : Qt::WindowModal);
}

void DataEditDialogHelper::init(AbstractDataEdit *edit)
{
    QDialog *dialog = this;

    // Create the main layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Add the DataEdit widget (the form to edit the object)
    layout->addWidget(edit->editWidget());

    m_errorOutput = new QLabel(this);
    m_errorOutput->setWordWrap(true);
    m_errorOutput->setStyleSheet("color: red");
    layout->addWidget(m_errorOutput);

    // Create a button box (Save and Cancel buttons)
    m_buttonBox = new QDialogButtonBox;
    m_buttonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    layout->addWidget(m_buttonBox);

    // Connect edit signals to the appropriate functions
    if (edit->editType() == AbstractDataEdit::WidgetEdit) {
        DataEdit *dataEdit = static_cast<DataEdit *>(edit);
        connect(dataEdit, &DataEdit::complete, this, &DataEditDialogHelper::updateError);
        connect(dataEdit, &DataEdit::complete, this, &DataEditDialogHelper::updateButtons);
    }

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

void DataEditDialogHelper::updateErrorState(bool show)
{
    if (show && !m_edit->isComplete()) {
        m_errorOutput->setText(m_edit->completionError());
        m_edit->clearCompletionError();
    } else {
        m_errorOutput->clear();
    }
}

void DataEditDialogHelper::updateButtonStates(bool saveable)
{
    if (operation() == AbstractDataEdit::ShowOperation) {
        m_buttonBox->setStandardButtons(QDialogButtonBox::Close);
    } else {
        QDialogButtonBox::StandardButtons buttons(QDialogButtonBox::Cancel);
        buttons.setFlag(QDialogButtonBox::Save, saveable || m_edit->isComplete());
        m_buttonBox->setStandardButtons(buttons);
    }
}

void DataEditDialogHelper::exec(const DataEditFinishedCallback &callback)
{
    d_ptr->finishCallback = callback;

    if (m_multiple)
        QWidget::show();
    else
        QDialog::open();
}

QWidget *DataEditDialogHelper::editWidget() const
{
    return const_cast<DataEditDialogHelper *>(this);
}

AbstractDataEdit::EditType DataEditDialogHelper::editType() const
{
    return m_edit->editType();
}

void DataEditDialogHelper::setVisible(bool v)
{
    if (v) {
        updateErrorState(false);
        updateButtonStates(true);
    }
    QDialog::setVisible(v);
}

void DataEditDialogHelper::clear()
{
    m_edit->clear();
}

void DataEditDialogHelper::render(const Jsoner::Object &object, Operation operation)
{
    m_edit->render(object, operation);
}

void DataEditDialogHelper::extract(Jsoner::Object &object, Operation operation) const
{
    m_edit->extract(object, operation);
}

bool DataEditDialogHelper::validateInput()
{
    return m_edit->validateInput();
}

void DataEditDialogHelper::makeWriteable(bool writeable)
{
    m_edit->makeWriteable(writeable);

    if (!writeable) {
        m_buttonBox->setStandardButtons(QDialogButtonBox::Close);
    } else {
        m_buttonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    }
}

} // namespace Widgetry
