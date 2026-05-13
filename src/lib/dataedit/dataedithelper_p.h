#ifndef DATAEDITHELPER_P_H
#define DATAEDITHELPER_P_H

#include <QtCore/qobject.h>

namespace Widgetry {

class DataEditHelper : public QObject
{
    Q_OBJECT

public:
    enum FieldMemberType {
        PropertyMember,
        SignalMember,
        DeduceMember
    };

    explicit DataEditHelper(QObject *parent = nullptr);

    bool registerField(QWidget *field, QWidget *edit);
    bool registerField(QWidget *field, const char *member, QWidget *edit);
    bool registerField(QWidget *field, const char *member, QWidget *edit, FieldMemberType type);

    static DataEditHelper *global();

private slots:
    void handleFieldChange();
    void handleFieldChange(bool);
    void handleFieldChange(int);
    void handleFieldChange(double);
    void handleFieldChange(const QString &);
    void handleFieldChange(const QDate &);
    void handleFieldChange(const QTime &);
    void handleFieldChange(const QDateTime &);

private:
    void emitCompleteChanged(QObject *field);
};

} // namespace Widgetry

#endif // DATAEDITHELPER_P_H
