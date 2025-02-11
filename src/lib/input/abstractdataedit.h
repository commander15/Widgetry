#ifndef WIDGETRY_ABSTRACTDATAEDIT_H
#define WIDGETRY_ABSTRACTDATAEDIT_H

#include <Widgetry/global.h>

#include <Jsoner/object.h>

#include <QtWidgets/qwidget.h>

#include <QtCore/qscopedpointer.h>

class QDialog;

namespace Widgetry {

class DataInterface;
class DataResponse;
typedef std::function<void(const Jsoner::Object &, int)> DataEditFinishedCallback;

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

    enum EditCode {
        Accepted = 1,
        Rejected = 0
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

    virtual DataInterface *interface() const;
    virtual void setInterface(DataInterface *interface);

    virtual QWidget *editWidget() const;
    virtual EditType editType() const;

    static QDialog *dialogFromEdit(AbstractDataEdit *edit, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    static AbstractDataEdit *editFromDialog(QDialog *dialog);

public:
    void show();
    void show(const Jsoner::Object &object);
    void add(const Jsoner::Object &object);
    void edit(const Jsoner::Object &object);

    virtual void reset();
    virtual void clear();

    virtual void exec(const DataEditFinishedCallback &onFinished);

protected:
    enum FieldMemberType {
        PropertyMember,
        SignalMember,
        DeduceMember
    };

    AbstractDataEdit(AbstractDataEditPrivate *d);

    template<typename Edit, typename Signal> void initEditing(Edit *edit, Signal &&signal)
    { QObject::connect(edit, signal, edit, [this](int result) { finishEditing(result); }); }

    virtual void render(const Jsoner::Object &object, Operation operation) = 0;
    virtual void extract(Jsoner::Object &object, Operation operation) const = 0;
    virtual bool validateInput() = 0;
    virtual void makeWriteable(bool writeable) = 0;

    void setCompletionError(const QString &str);
    void clearCompletionError();

    virtual void finishEditing(int result = Accepted);
    virtual void handleResponse(const DataResponse &response);

    QScopedPointer<AbstractDataEditPrivate> d_ptr;

    friend class DataEditDialogHelper;
    friend class DataWindow;
};

} // namespace Widgetry

inline size_t qHash(const QVariant &key, size_t seed = 0)
{ return qHash(key.toString()); }

#endif // WIDGETRY_ABSTRACTDATAEDIT_H
