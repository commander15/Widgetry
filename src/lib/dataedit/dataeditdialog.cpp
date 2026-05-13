#include "dataeditdialog.h"

#include <Widgetry/private/dataedithelperdialog_p.h>

namespace Widgetry {

DataEditDialog::DataEditDialog(QWidget *parent, Qt::WindowFlags flags)
    : Dialog(parent, flags)
    , AbstractDataEdit(this)
{
    bindEnd(this, &QDialog::finished);
}

DataEditDialog::~DataEditDialog()
{
}

AbstractDataEdit *DataEditDialog::dialogEdit(DataEditDialog *dialog)
{
    if (dialog->inherits("Widgetry::DataEditDialogHelper"))
        return static_cast<DataEditDialogHelper *>(dialog)->wrappedEdit();
    else
        return nullptr;
}

DataEditDialog *DataEditDialog::fromEdit(AbstractDataEdit *edit, QWidget *parent, Qt::WindowFlags flags)
{
    DataEditDialogHelper *dialog = new DataEditDialogHelper(parent, flags);
    dialog->init(edit);
    return dialog;
}

void DataEditDialog::render(const Jsoner::Object &object)
{
    Q_UNUSED(object);
}

void DataEditDialog::extract(Jsoner::Object &object) const
{
    Q_UNUSED(object);
}

void DataEditDialog::makeVisible(bool visibible, int result)
{
    if (visibible && !isVisible()) {
        open();
        return;
    }

    if (!visibible && isVisible())
        done(result);
}

} // namespace Widgetry
