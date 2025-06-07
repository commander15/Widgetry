#ifndef WIDGETRY_FORMDATAEDIT_H
#define WIDGETRY_FORMDATAEDIT_H

#include <Widgetry/global.h>
#include <Widgetry/dataedit.h>

class QFormLayout;

namespace Widgetry {

class WIDGETRY_EXPORT FormDataEdit : public DataEdit
{
    Q_OBJECT

public:
    FormDataEdit(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~FormDataEdit();

    bool isRowVisible(int row) const;
    void showRow(int row);
    void hideRow(int row);
    void setRowVisible(int row, bool visible);

    int addRow(const QString &label, QWidget *field);
    int addRow(const QString &label, QLayout *field);
    int addRow(QWidget *label, QLayout *field);
    int addRow(QWidget *label, QWidget *field);
    int addRow(QWidget *widget);
    int addRow(QLayout *layout);

    void insertRow(int row, const QString &label, QWidget *field);
    void insertRow(int row, const QString &label, QLayout *field);
    void insertRow(int row, QWidget *label, QLayout *field);
    void insertRow(int row, QWidget *label, QWidget *field);
    void insertRow(int row, QWidget *widget);
    void insertRow(int row, QLayout *layout);

    void removeRow(int row);

    int rowCount() const;

    QFormLayout *layout() const;

public slots:
    void clear() override;

protected:
    void render(const Jsoner::Object &object, Operation operation) override;
    void extract(Jsoner::Object &object, Operation operation) const override;
    void makeWriteable(bool writeable) override;

    static QString fieldName(QWidget *widget);
    static void setFieldName(QWidget *widget, const QString &name);
    static QVariant fieldValue(QWidget *widget);
    static bool setFieldValue(QWidget *widget, const QVariant &value);
    static bool clearFieldValue(QWidget *widget);
    static bool makeFieldWriteable(QWidget *widget, bool writeable = true);
};

} // namespace Widgetry

#endif // WIDGETRY_FORMDATAEDIT_H
