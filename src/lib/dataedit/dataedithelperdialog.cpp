#include "dataedithelperdialog_p.h"

#include <Widgetry/dataedit.h>

#include <Widgetry/private/abstractdataedit_p.h>

#include <QtWidgets/qboxlayout.h>
#include <qpushbutton.h>

namespace Widgetry {

DataEditDialogHelper::DataEditDialogHelper(QWidget *parent, Qt::WindowFlags flags, bool multiple)
    : DataEditDialog(parent, flags | (multiple ? Qt::Window : Qt::WindowFlags()))
    , m_multiple(multiple)
{
    setWindowModality(multiple ? Qt::NonModal : Qt::WindowModal);

    m_errorCleanTimer.setInterval(3000);
    m_errorCleanTimer.setSingleShot(true);

    m_callback = [this](const Jsoner::Object &, int result) {
        end(result);
    };
}

void DataEditDialogHelper::init(AbstractDataEdit *edit)
{
    QDialog *dialog = this;

    dialog->setWindowTitle(edit->editWidget()->windowTitle());
    connect(edit->editWidget(), &QWidget::windowTitleChanged, dialog, &QWidget::setWindowTitle);

    // Create the main layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Add the DataEdit widget (the form to edit the object)
    layout->addWidget(edit->editWidget());

    m_errorOutput = new QLabel(this);
    m_errorOutput->setWordWrap(true);
    m_errorOutput->setStyleSheet("color: red");
    m_errorOutput->hide();
    layout->addWidget(m_errorOutput);

    connect(&m_errorCleanTimer, &QTimer::timeout, m_errorOutput, &QLabel::hide);

    // Create a button box (Save and Cancel buttons)
    m_buttonBox = new QDialogButtonBox;
    m_buttonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    layout->addWidget(m_buttonBox);

    // Connect edit signals to the appropriate functions
    if (edit->editType() == AbstractDataEdit::WidgetEdit) {
        DataEdit *dataEdit = static_cast<DataEdit *>(edit);
        connect(dataEdit, &DataEdit::completeChanged, this, &DataEditDialogHelper::checkInput);
        connect(dataEdit, &DataEdit::editingFinished, this, [this](int result) { end(result); });
    }

    // Connect button signals to the appropriate slots
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [&] {
        if (hasValidInput()) {
            accept();
        } else {
            checkInput();
        }
    });
    connect(m_buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    // Set the dialog layout
    dialog->setLayout(layout);
    dialog->setMinimumHeight(layout->minimumSize().height() + m_errorOutput->height());

    edit->d_ptr->finishCallback = m_callback;
    m_edit = edit;
}

void DataEditDialogHelper::updateErrorState(bool show)
{
    if (show)
        validateInput();
    else
        m_errorOutput->hide();
}

void DataEditDialogHelper::updateButtonStates(bool saveable)
{
    if (operation() == AbstractDataEdit::ShowOperation) {
        m_buttonBox->setStandardButtons(QDialogButtonBox::Close);
    } else {
        QDialogButtonBox::StandardButtons buttons(QDialogButtonBox::Cancel);
        buttons.setFlag(QDialogButtonBox::Save, saveable || m_edit->hasValidInput());
        m_buttonBox->setStandardButtons(buttons);
    }
}

void DataEditDialogHelper::run(const DataEditFinishedCallback &callback)
{
    WIDGETRY_D(AbstractDataEdit);
    d->finishCallback = callback;

    AbstractDataEditPrivate *edit = m_edit->d_ptr.get();
    edit->finishCallback = m_callback;
    edit->browser = d_ptr->browser;

    if (m_multiple)
        QWidget::show();
    else
        QDialog::open();
}

void DataEditDialogHelper::checkInput()
{
    bool writable = (operation() != ShowOperation);
    if (!writable) {
        m_buttonBox->setStandardButtons(QDialogButtonBox::Close);
    } else {
        bool valid = hasValidInput();
        if (!valid) {
            const QString str = validationError();
            if (!str.isEmpty()) {
                m_errorOutput->setText(str);
                m_errorOutput->show();
                m_errorCleanTimer.start();
            }
        }

        m_buttonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
        if (sender() == m_edit->editWidget())
            m_buttonBox->button(QDialogButtonBox::Save)->setEnabled(valid);
    }
}

void DataEditDialogHelper::clear()
{
    m_edit->clear();
}

void DataEditDialogHelper::render(const Jsoner::Object &object)
{
    m_edit->setObject(object, operation());
}

void DataEditDialogHelper::extract(Jsoner::Object &object) const
{
    object = m_edit->object();
}

bool DataEditDialogHelper::validateInput()
{
    if (!m_edit->hasValidInput()) {
        setValidationError(m_edit->validationError());
        return false;
    }

    return true;
}

void DataEditDialogHelper::makeWriteable(bool writable)
{
    m_edit->setReadOnly(!writable);
    checkInput();
}

} // namespace Widgetry
