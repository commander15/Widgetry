#include "abstractdataeditfactory.h"
#include "abstractdataeditfactory_p.h"

#include <Widgetry/datainterface.h>
#include <Widgetry/datawindow.h>

#include <Widgetry/private/dataeditdialog_p.h>

namespace Widgetry {

AbstractDataEditFactory::AbstractDataEditFactory()
    : d_ptr(new AbstractDataEditFactoryPrivate())
{
}

bool AbstractDataEditFactory::isDialogCreationSupported() const
{
    return d_ptr->allowDialogCreation;
}

void AbstractDataEditFactory::enableDialogCreation()
{
    d_ptr->allowDialogCreation = true;
}

AbstractDataEditFactory::~AbstractDataEditFactory() = default;

AbstractDataEdit *AbstractDataEditFactory::create(const Jsoner::Object &object, AbstractDataEdit::Operation operation, QWidget *parent)
{
    AbstractDataEdit *edit = createEdit(parent);
    edit->setObject(object, operation);

    if (edit->editType() == AbstractDataEdit::WindowEdit && parent && parent->inherits("Widgetry::DataInterface")) {
        DataInterface *interface = static_cast<DataInterface *>(parent);
        DataWindow *window = static_cast<DataWindow *>(edit);
        window->registerAdd(interface->editCallback(AbstractDataEdit::AddOperation));
        window->registerEdit(interface->editCallback(AbstractDataEdit::EditOperation));
        return window;
    }

    if (d_ptr->allowDialogCreation) {
        DataEditDialogHelper *dialog = new DataEditDialogHelper(parent, Qt::Window);
        dialog->init(edit);
        return dialog;
    }

    return edit;
}

} // namespace Widgetry
