#include "datawindow.h"
#include "datawindow_p.h"

#include <Widgetry/private/abstractdataedit_p.h>

namespace Widgetry {

DataWindow::DataWindow(QWidget *parent, Qt::WindowFlags flags)
    : DataWindow(QByteArray(), parent, flags)
{
}

DataWindow::DataWindow(const QByteArray &id, QWidget *parent, Qt::WindowFlags flags)
    : DataEdit(new DataWindowPrivate(this, id), parent, Qt::Window | flags)
{
}

DataWindow::~DataWindow()
{
}

void DataWindow::registerAdd(const DataEditFinishedCallback &callback)
{
    WIDGETRY_D(DataWindow);
    d->addCallback = callback;
}

void DataWindow::registerEdit(const DataEditFinishedCallback &callback)
{
    WIDGETRY_D(DataWindow);
    d->editCallback = callback;
}

void DataWindow::begin()
{
    WIDGETRY_D(DataWindow);

    if (!d->browserSet) {
        std::for_each(d->edits.constBegin(), d->edits.constEnd(), [this](const DataWindowPrivate::Edit &edit) {
            edit.edit->setBrowser(browser());
        });
        d->browserSet = true;
    }

    DataEdit::begin();
}

void DataWindow::end(int result)
{
    WIDGETRY_D(DataWindow);

    if (d->edit->finishCallback != nullptr) {
        DataEdit::end(result);
        return;
    }

    switch (d->edit->operation) {
    case AddOperation:
        if (d->addCallback)
            d->addCallback(object(), result);
        break;

    case EditOperation:
        if (d->editCallback)
            d->editCallback(object(), result);
        break;

    default:
        break;
    }
}

void DataWindow::render(const Jsoner::Object &object)
{
    WIDGETRY_D(DataWindow);
    std::for_each(d->edits.constBegin(), d->edits.constEnd(), [&object](const DataWindowPrivate::Edit &edit) {
        if (edit.fieldName.isEmpty())
            edit.edit->setObject(object);
        else
            edit.edit->setObject(object.object(edit.fieldName));
    });
}

void DataWindow::extract(Jsoner::Object &object) const
{
    WIDGETRY_D(DataWindow);
    std::for_each(d->edits.constBegin(), d->edits.constEnd(), [&object](const DataWindowPrivate::Edit &edit) {
        if (edit.fieldName.isEmpty()) {
            edit.edit->extract(object);
        } else {
            Jsoner::Object subObject = object.object(edit.fieldName);
            edit.edit->extract(subObject);
            object.put(edit.fieldName, subObject);
        }
    });
}

bool DataWindow::validateInput()
{
    WIDGETRY_D(DataWindow);
    auto e = std::find_if(d->edits.constBegin(), d->edits.constEnd(), [](const DataWindowPrivate::Edit &edit) {
        return !edit.edit->hasValidInput();
    });

    if (e != d->edits.end()) {
        setValidationError(e->edit->validationError());
        return false;
    }

    return true;
}

void DataWindow::makeWriteable(bool writeable)
{
    WIDGETRY_D(DataWindow);
    std::for_each(d->edits.constBegin(), d->edits.constEnd(), [&writeable](const DataWindowPrivate::Edit &edit) {
        edit.edit->makeWriteable(writeable);
    });
}

void DataWindow::registerEdit(AbstractDataEdit *edit)
{
    registerEdit(QString(), edit);
}

void DataWindow::registerEdit(const QString &field, AbstractDataEdit *edit)
{
    WIDGETRY_D(DataWindow);

    auto e = std::find_if(d->edits.constBegin(), d->edits.constEnd(), [edit](const DataWindowPrivate::Edit &windowEdit) {
        return windowEdit.edit == edit;
    });

    if (e != d->edits.end())
        return;

    DataWindowPrivate::Edit windowEdit;
    windowEdit.fieldName = field;
    windowEdit.edit = edit;
    d->edits.append(windowEdit);
}

DataWindowPrivate::DataWindowPrivate(DataWindow *q, const QByteArray &id)
    : Widgetry::DataEditPrivate(q, id)
    , browserSet(false)
{
}

} // namespace Widgetry
