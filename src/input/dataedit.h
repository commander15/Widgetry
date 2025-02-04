#ifndef WIDGETRY_DATAEDIT_H
#define WIDGETRY_DATAEDIT_H

#include <Widgetry/global.h>
#include <Widgetry/abstractdataedit.h>

#include <QtWidgets/qwidget.h>

#include <Jsoner/object.h>

namespace Widgetry {

class DataEditPrivate;
class WIDGETRY_EXPORT DataEdit : public QWidget, public AbstractDataEdit
{
    Q_OBJECT

public:
    DataEdit(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataEdit();

    QWidget *editWidget() const override;
    EditType editType() const override;

    static QDialog *dialogFromEdit(DataEdit *edit, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

public slots:
    void show(const Jsoner::Object &object) { AbstractDataEdit::show(object); }
    void add(const Jsoner::Object &object) { AbstractDataEdit::add(object); }
    void edit(const Jsoner::Object &object) { AbstractDataEdit::edit(object); }
    void clear() override;

signals:
    void complete();

protected:
    enum FieldMemberType {
        PropertyMember,
        SignalMember,
        DeduceMember
    };

    DataEdit(DataEditPrivate *d, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    bool validateInput() override;

    bool registerField(QWidget *field);
    bool registerField(QWidget *field, const char *member);
    bool registerField(QWidget *field, const char *member, FieldMemberType type);

    using AbstractDataEdit::d_ptr;

private slots:
    void handleFieldChange();
    void handleFieldChange(bool);
    void handleFieldChange(int);
    void handleFieldChange(double);
    void handleFieldChange(const QString &);
    void handleFieldChange(const QDate &);
    void handleFieldChange(const QTime &);
    void handleFieldChange(const QDateTime &);

    friend class DataEditDialogHelper;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAEDIT_H
