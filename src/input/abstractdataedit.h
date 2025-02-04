#ifndef WIDGETRY_ABSTRACTDATAEDIT_H
#define WIDGETRY_ABSTRACTDATAEDIT_H

#include <Widgetry/global.h>

#include <QtWidgets/qwidget.h>

#include <QtCore/qscopedpointer.h>

namespace Jsoner {
class Object;
}

class QDialog;

namespace Widgetry {

class AbstractDataEditPrivate;
class WIDGETRY_EXPORT AbstractDataEdit
{
public:
    enum Operation {
        ShowOperation,
        AddOperation,
        EditOperation
    };

    enum EditType {
        WindowEdit,
        WidgetEdit,
        CustomEdit
    };

    explicit AbstractDataEdit();
    virtual ~AbstractDataEdit();

    Jsoner::Object object() const;
    Operation operation() const;
    void setObject(const Jsoner::Object &object, Operation operation);

    bool isComplete() const;
    QString completionError() const;

    bool isReadOnly() const;
    void setReadOnly(bool r = true);

    virtual QWidget *editWidget() const = 0;
    virtual EditType editType() const;

    static QDialog *dialogFromEdit(AbstractDataEdit *edit, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

public:
    void show(const Jsoner::Object &object);
    void add(const Jsoner::Object &object);
    void edit(const Jsoner::Object &object);
    virtual void clear();

protected:
    enum FieldMemberType {
        PropertyMember,
        SignalMember,
        DeduceMember
    };

    AbstractDataEdit(AbstractDataEditPrivate *d);

    virtual void render(const Jsoner::Object &object, Operation operation) = 0;
    virtual void extract(Jsoner::Object &object, Operation operation) const = 0;
    virtual bool validateInput() = 0;
    virtual void makeWriteable(bool writeable) = 0;

    void setCompletionError(const QString &str);
    void clearCompletionError();

    QScopedPointer<AbstractDataEditPrivate> d_ptr;

    friend class DataEditDialogHelper;
};

} // namespace Widgetry

#endif // WIDGETRY_ABSTRACTDATAEDIT_H
