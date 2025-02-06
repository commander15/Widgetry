#include "datawindow.h"
#include "datawindow_p.h"

namespace Widgetry {

DataWindow::DataWindow(const QByteArray &id, QWidget *parent, Qt::WindowFlags flags)
    : UserInterface(new DataWindowPrivate(this, id), parent, Qt::Window | flags)
{
    initEditing(this, &DataWindow::editingFinished);
}

DataWindow::~DataWindow()
{
}

QWidget *DataWindow::editWidget() const
{
    return const_cast<DataWindow *>(this);
}

void DataWindow::show()
{
    QWidget::show();
}

void DataWindow::show(const Jsoner::Object &object)
{
    setObject(object, ShowOperation);
    QWidget::show();
}

void DataWindow::add(const Jsoner::Object &object)
{
    setObject(object, AddOperation);
    QWidget::show();
}

void DataWindow::edit(const Jsoner::Object &object)
{
    setObject(object, EditOperation);
    QWidget::show();
}

void DataWindow::clear()
{
    WIDGETRY_D(DataWindow);
    std::for_each(d->edits.begin(), d->edits.end(), [](const DataWindowPrivate::Edit &edit) {
        edit.edit->clear();
    });

    AbstractDataEdit::clear();
    UserInterface::cleanupUi();
}

void DataWindow::render(const Jsoner::Object &object, Operation operation)
{
    WIDGETRY_D(DataWindow);
    std::for_each(d->edits.begin(), d->edits.end(), [&object, &operation](const DataWindowPrivate::Edit &edit) {
        if (edit.fieldName.isEmpty())
            edit.edit->render(object, operation);
        else
            edit.edit->render(object.object(edit.fieldName), operation);
    });
}

void DataWindow::extract(Jsoner::Object &object, Operation operation) const
{
    WIDGETRY_D(DataWindow);
    std::for_each(d->edits.begin(), d->edits.end(), [&object, &operation](const DataWindowPrivate::Edit &edit) {
        if (edit.fieldName.isEmpty())
            edit.edit->extract(object, operation);
        else {
            Jsoner::Object subObject = object.object(edit.fieldName);
            edit.edit->extract(subObject, operation);
            object.insert(edit.fieldName, subObject);
        }
    });
}

bool DataWindow::validateInput()
{
    WIDGETRY_D(DataWindow);
    auto e = std::find_if(d->edits.begin(), d->edits.end(), [](const DataWindowPrivate::Edit &edit) {
        return !edit.edit->validateInput();
    });

    if (e != d->edits.end()) {
        setCompletionError(e->edit->completionError());
        return false;
    }

    return true;
}

void DataWindow::makeWriteable(bool writeable)
{
    WIDGETRY_D(DataWindow);
    std::for_each(d->edits.begin(), d->edits.end(), [&writeable](const DataWindowPrivate::Edit &edit) {
        edit.edit->makeWriteable(writeable);
    });
}

Widgetry::AbstractDataEdit::EditType DataWindow::editType() const
{
    return WindowEdit;
}

void DataWindow::registerEdit(AbstractDataEdit *edit)
{
    registerEdit(QString(), edit);
}

void DataWindow::registerEdit(const QString &field, AbstractDataEdit *edit)
{
    WIDGETRY_D(DataWindow);

    auto e = std::find_if(d->edits.begin(), d->edits.end(), [edit](const DataWindowPrivate::Edit &windowEdit) {
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
    : UserInterfacePrivate(q, id)
{
}

} // namespace Widgetry
