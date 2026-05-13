#include "abstractdataeditfactory.h"
#include "abstractdataeditfactory_p.h"

#include <QtWidgets/qwidget.h>

#include <Widgetry/databrowser.h>
#include <Widgetry/datawindow.h>
#include <Widgetry/dataedit.h>
#include <Widgetry/private/dataedithelperdialog_p.h>

namespace Widgetry {

AbstractDataEditFactory::AbstractDataEditFactory()
    : d_ptr(new AbstractDataEditFactoryPrivate())
{
}

AbstractDataEditFactory::~AbstractDataEditFactory()
{
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
    d_ptr->singleInstance = (d_ptr->maxCount == 1);
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

    if (edit == nullptr && (!d_ptr->canCreateEdit() && d_ptr->cleanupInvisibleEdits() == 0))
        return nullptr;

    edit = createEdit(parent);
    if (edit == nullptr)
        return nullptr;

    DataBrowser *browser;
    if (parent && parent->inherits("Widgetry::DataBrowser"))
        browser = static_cast<DataBrowser *>(parent);
    else
        browser = nullptr;

    if (browser != nullptr)
        edit->setBrowser(browser);

    if (edit->editType() == AbstractDataEdit::WindowEdit && browser != nullptr) {
        DataWindow *window = static_cast<DataWindow *>(edit);
        window->registerAdd(browser->editCallback(AbstractDataEdit::AddOperation));
        window->registerEdit(browser->editCallback(AbstractDataEdit::EditOperation));
    }

    edit = createContainer(edit, parent, d_ptr->containerFlags);
    edit->setObject(object, operation);
    d_ptr->registerEdit(edit);
    return edit;
}

AbstractDataEdit *AbstractDataEditFactory::createContainer(AbstractDataEdit *edit, QWidget *parent, Qt::WindowFlags flags)
{
    if (edit->editType() == AbstractDataEdit::WidgetEdit && d_ptr->allowDialogCreation && !edit->editWidget()->isWindow()) {
        return DataEditDialog::fromEdit(edit, parent, flags);
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
    edits.append(edit);

    connect(w, &QObject::destroyed, this, [this, edit] { unregisterEdit(edit); });
}

void AbstractDataEditFactoryPrivate::unregisterEdit(AbstractDataEdit *edit)
{
    edits.removeOne(edit);
}

int AbstractDataEditFactoryPrivate::cleanupInvisibleEdits()
{
    int count = 0;

    std::for_each(edits.begin(), edits.end(), [&count](AbstractDataEdit *edit) {
        QWidget *w = edit->editWidget();
        if (!w->isVisible()) {
            ++count;
            w->deleteLater();
        }
    });

    return count;
}

} // namespace Widgetry
