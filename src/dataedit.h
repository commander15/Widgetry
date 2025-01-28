#ifndef WIDGETRY_DATAEDIT_H
#define WIDGETRY_DATAEDIT_H

#include <Widgetry/global.h>

#include <QtWidgets/qwidget.h>

#include <GenO/object.h>

namespace Widgetry {

class DataEditPrivate;
class WIDGETRY_EXPORT DataEdit : public QWidget
{
    Q_OBJECT

public:
    enum Operation {
        ShowOperation,
        AddOperation,
        EditOperation
    };

    DataEdit(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataEdit();

    GenO::Object object() const;
    Operation operation() const;
    void setObject(const GenO::Object &object, Operation operation);

    bool isComplete() const;
    QString completionError() const;

    bool isReadOnly() const;
    void setReadOnly(bool r = true);

    static QDialog *dialogFromEdit(DataEdit *edit, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

public slots:
    void show(const GenO::Object &object);
    void add(const GenO::Object &object);
    void edit(const GenO::Object &object);

    virtual void clear();

signals:
    void complete();

protected:
    enum FieldMemberType {
        PropertyMember,
        SignalMember,
        DeduceMember
    };

    DataEdit(DataEditPrivate *d, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    virtual void render(const GenO::Object &object, Operation operation) = 0;
    virtual void extract(GenO::Object &object, Operation operation) const = 0;
    virtual bool validateInput();
    virtual void makeWriteable(bool writeable) = 0;

    void setCompletionError(const QString &str);
    void clearCompletionError();

    bool registerField(QWidget *field);
    bool registerField(QWidget *field, const char *member);
    bool registerField(QWidget *field, const char *member, FieldMemberType type);

    QScopedPointer<DataEditPrivate> d_ptr;

private slots:
    void handleFieldChange();
    void handleFieldChange(bool);
    void handleFieldChange(int);
    void handleFieldChange(double);
    void handleFieldChange(const QString &);

    friend class DataEditDialogHelper;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAEDIT_H
