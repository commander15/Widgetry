#include "abstractdataeditfactory.h"
#include "abstractdataeditfactory_p.h"

#include <Widgetry/datainterface.h>
#include <Widgetry/datawindow.h>

#include <Widgetry/private/dataeditdialog_p.h>

#include <QtWidgets/qwidget.h>

namespace Widgetry {

AbstractDataEditFactory::AbstractDataEditFactory()
    : d_ptr(new AbstractDataEditFactoryPrivate())
{
}

void AbstractDataEditFactory::setMainField(const QString &field)
{
    d_ptr->field = field;
}

void AbstractDataEditFactory::setMaxCount(int count)
{
    d_ptr->maxCount = (count > 0 ? count : 1);
}

bool AbstractDataEditFactory::isDialogCreationSupported() const
{
    return d_ptr->allowDialogCreation;
}

void AbstractDataEditFactory::enableDialogCreation()
{
    d_ptr->allowDialogCreation = true;
}

void AbstractDataEditFactory::disableDialogCreation()
{
    d_ptr->allowDialogCreation = false;
}

AbstractDataEditFactory::~AbstractDataEditFactory()
{
    while (d_ptr->edits.size() > 0)
        delete d_ptr->edits.takeFirst();
}

AbstractDataEdit *AbstractDataEditFactory::create(const Jsoner::Object &object, AbstractDataEdit::Operation operation, QWidget *parent)
{
    const QVariant key = (operation == AbstractDataEdit::AddOperation ? QVariant() : object.variant(d_ptr->field));

    AbstractDataEdit *edit = d_ptr->editForKey(key);

    if (!edit && (d_ptr->canCreateEdit() || d_ptr->cleanupInvisibleEdits() > 0)) {
        edit = createEdit(parent);

        if (edit)
            d_ptr->registerEdit(edit);
    }

    if (!edit)
        return nullptr;

    edit->setObject(object, operation);

    if (edit->editType() == AbstractDataEdit::WindowEdit && parent && parent->inherits("Widgetry::DataInterface")) {
        DataInterface *interface = static_cast<DataInterface *>(parent);
        DataWindow *window = static_cast<DataWindow *>(edit);
        window->registerAdd(interface->editCallback(AbstractDataEdit::AddOperation));
        window->registerEdit(interface->editCallback(AbstractDataEdit::EditOperation));
        return window;
    }

    if (edit->editType() == AbstractDataEdit::WidgetEdit && d_ptr->allowDialogCreation) {
        DataEditDialogHelper *dialog = new DataEditDialogHelper(parent, Qt::Window);
        dialog->init(edit);
        return dialog;
    }

    return edit;
}

AbstractDataEdit *AbstractDataEditFactoryPrivate::editForKey(const QVariant &key) const
{
    if (maxCount == 1)
        return !edits.isEmpty() ? edits.first() : nullptr;

    auto e = std::find_if(edits.begin(), edits.end(), [this, &key](const AbstractDataEdit *edit) {
        return edit->object().variant(field) == key;
    });

    return (e != edits.end() ? *e : nullptr);
}

bool AbstractDataEditFactoryPrivate::canCreateEdit() const
{
    return edits.size() < maxCount;
}

void AbstractDataEditFactoryPrivate::registerEdit(AbstractDataEdit *edit)
{
    edits.append(edit);

    QWidget *w = edit->editWidget();
    QObject::connect(w, &QObject::destroyed, w, [this, edit] { edits.removeOne(edit); });
}

int AbstractDataEditFactoryPrivate::cleanupInvisibleEdits()
{
    QList<AbstractDataEdit *> invisibleEdits;

    std::for_each(edits.begin(), edits.end(), [&invisibleEdits](AbstractDataEdit *edit) {
        QWidget *w = edit->editWidget();
        if (!w->isVisible())
            invisibleEdits.append(edit);
    });

    for (AbstractDataEdit *edit : invisibleEdits)
        delete edit;

    return invisibleEdits.size();
}

} // namespace Widgetry
