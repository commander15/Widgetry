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

    QWidget *editWidget() const override final;
    EditType editType() const override final;

public slots:
    void show();
    void show(const Jsoner::Object &object);
    void add(const Jsoner::Object &object);
    void edit(const Jsoner::Object &object);

    void reset() override;
    void clear() override;

signals:
    void complete();
    void editingFinished(int result = Accepted);

protected:
    enum FieldMemberType {
        PropertyMember,
        SignalMember,
        DeduceMember
    };

    DataEdit(DataEditPrivate *d, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

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
