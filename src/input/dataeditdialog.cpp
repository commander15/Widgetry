#include "dataeditdialog_p.h"

#include <Widgetry/dataedit.h>

#include <QtWidgets/qboxlayout.h>

namespace Widgetry {

DataEditDialogHelper::DataEditDialogHelper(QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
{
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
    m_buttonBox = new QDialogButtonBox();
    QDialogButtonBox::StandardButtons boxButtons = QDialogButtonBox::Save | QDialogButtonBox::Cancel;
    m_buttonBox->setStandardButtons(boxButtons);
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
    if (m_edit->operation() == AbstractDataEdit::ShowOperation) {
        m_buttonBox->setStandardButtons(QDialogButtonBox::Close);
    } else {
        QDialogButtonBox::StandardButtons buttons(QDialogButtonBox::Cancel);
        buttons.setFlag(QDialogButtonBox::Save, saveable || m_edit->isComplete());
        m_buttonBox->setStandardButtons(buttons);
    }
}

void DataEditDialogHelper::setVisible(bool v)
{
    if (v) {
        updateErrorState(false);
        updateButtonStates(true);
    }
    QDialog::setVisible(v);
}

} // namespace Widgetry
