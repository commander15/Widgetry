#ifndef WIDGETRY_ABSTRACTDATAEDIT_H
#define WIDGETRY_ABSTRACTDATAEDIT_H

#include <Widgetry/global.h>

#include <Jsoner/object.h>

#include <QtCore/qscopedpointer.h>

namespace DataGate {
class DataResponse;
}

class QDialog;

namespace Widgetry {

class DataBrowser;
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
        WidgetEdit,
        DialogEdit,
        CustomEdit,
        WindowEdit
    };

    enum EditCode {
        Success = 1,
        Failure = 0
    };

    explicit AbstractDataEdit(QWidget *widget);
    virtual ~AbstractDataEdit();

    Jsoner::Object object() const;
    Operation operation() const;
    void setObject(const Jsoner::Object &object);
    void setObject(const Jsoner::Object &object, Operation operation);

    virtual void reset();
    virtual void clear();

    bool hasValidInput() const;
    QString validationError() const;

    bool isReadOnly() const;
    void setReadOnly(bool r = true);

    virtual void begin();
    virtual void end(int result = Success);
    virtual void run(const DataEditFinishedCallback &onFinished);

    DataBrowser *browser() const;
    void setBrowser(DataBrowser *browser);

    QWidget *editWidget() const;
    virtual EditType editType() const { return CustomEdit; }

    static QString fieldName(QWidget *widget);
    static void setFieldName(QWidget *widget, const QString &name);

    static QVariant fieldValue(QWidget *widget);
    static bool setFieldValue(QWidget *widget, const QVariant &value);
    static bool clearFieldValue(QWidget *widget);

    static bool makeFieldWriteable(QWidget *widget, bool writeable = true);

protected:
    enum FieldMemberType {
        PropertyMember,
        SignalMember,
        DeduceMember
    };

    template<typename Edit, typename Signal> void bindBegin(Edit *edit, Signal &&signal)
    { QObject::connect(edit, signal, edit, [this] { begin(); }); }

    template<typename Edit, typename Signal> void bindEnd(Edit *edit, Signal &&signal)
    { QObject::connect(edit, signal, edit, [this](int result) { end(result); }); }

    bool registerField(QWidget *field);
    bool registerField(QWidget *field, const char *member);
    bool registerField(QWidget *field, const char *member, FieldMemberType type);

    void setValidationError(const QString &str);
    void clearValidationError();

    virtual void render(const Jsoner::Object &object) = 0;
    virtual void extract(Jsoner::Object &object) const = 0;
    virtual bool validateInput();
    virtual void makeWriteable(bool writable);
    virtual void makeVisible(bool visibible, int result = Success);

    QScopedPointer<AbstractDataEditPrivate> d_ptr;

    friend class DataEditDialogHelper;
    friend class DataWindow;
};

} // namespace Widgetry

inline size_t qHash(const QVariant &key, size_t seed = 0)
{ return qHash(key.toString()); }

#endif // WIDGETRY_ABSTRACTDATAEDIT_H
