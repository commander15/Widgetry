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

AbstractDataEditFactory::~AbstractDataEditFactory()
{
    while (d_ptr->edits.size() > 0)
        delete d_ptr->edits.takeFirst();
}

void AbstractDataEditFactory::setMainField(const QString &field)
{
    d_ptr->field = field;
}

void AbstractDataEditFactory::setSingleInstance(bool single)
{
    d_ptr->singleInstance = single;
    d_ptr->maxCount = 1;
}

void AbstractDataEditFactory::setMaxCount(int count)
{
    d_ptr->maxCount = (count > 0 ? count : 1);
    d_ptr->singleInstance = false;
}

void AbstractDataEditFactory::setContainerFlags(Qt::WindowFlags flags)
{
    d_ptr->containerFlags = flags;
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

int AbstractDataEditFactory::editCount() const
{
    return d_ptr->edits.count();
}

QList<AbstractDataEdit *> AbstractDataEditFactory::edits() const
{
    return d_ptr->edits;
}

AbstractDataEdit *AbstractDataEditFactory::create(const Jsoner::Object &object, AbstractDataEdit::Operation operation, QWidget *parent)
{
    const QVariant key = (operation == AbstractDataEdit::AddOperation ? QVariant() : object.variant(d_ptr->field));

    AbstractDataEdit *edit = d_ptr->editForKey(key);

    if (!edit && (d_ptr->canCreateEdit() || d_ptr->cleanupInvisibleEdits() > 0)) {
        edit = createEdit(parent);
        if (edit) {
            edit = createContainer(edit, parent, d_ptr->containerFlags);
            d_ptr->registerEdit(edit);
        }
    }

    if (edit)
        edit->setObject(object, operation);

    return edit;
}

AbstractDataEdit *AbstractDataEditFactory::createContainer(AbstractDataEdit *edit, QWidget *parent, Qt::WindowFlags flags)
{
    if (edit->editType() == AbstractDataEdit::WindowEdit && parent && parent->inherits("Widgetry::DataInterface")) {
        DataInterface *interface = static_cast<DataInterface *>(parent);
        DataWindow *window = static_cast<DataWindow *>(edit);
        window->registerAdd(interface->editCallback(AbstractDataEdit::AddOperation));
        window->registerEdit(interface->editCallback(AbstractDataEdit::EditOperation));
        return window;
    }

    if (edit->editType() == AbstractDataEdit::WidgetEdit && d_ptr->allowDialogCreation) {
        DataEditDialogHelper *dialog = new DataEditDialogHelper(parent, flags, !d_ptr->singleInstance);
        dialog->init(edit);
        return dialog;
    }

    return edit;
}

AbstractDataEdit *AbstractDataEditFactoryPrivate::editForKey(const QVariant &key) const
{
    if (singleInstance)
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
    QWidget *w = edit->editWidget();
    QObject::connect(w, &QObject::destroyed, w, [this, edit] { unregisterEdit(edit); });

    edits.append(edit);
}

void AbstractDataEditFactoryPrivate::unregisterEdit(AbstractDataEdit *edit)
{
    edits.removeOne(edit);
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
