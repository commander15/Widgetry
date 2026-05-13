#ifndef WIDGETRY_DATAEDITDIALOG_H
#define WIDGETRY_DATAEDITDIALOG_H

#include <Widgetry/dialog.h>
#include <Widgetry/abstractdataedit.h>

namespace Widgetry {

class DataEditDialog : public Dialog, public AbstractDataEdit
{
    Q_OBJECT

public:
    explicit DataEditDialog(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataEditDialog();

    EditType editType() const override final
    { return DialogEdit; }

    static AbstractDataEdit *dialogEdit(DataEditDialog *dialog);
    static DataEditDialog *fromEdit(AbstractDataEdit *edit, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

public slots:
    void begin() override { AbstractDataEdit::begin(); }
    void end(int result = Success) override { AbstractDataEdit::end(result); }
    void reset() override { AbstractDataEdit::reset(); }
    void clear() override { AbstractDataEdit::clear(); }

signals:
    void completeChanged();

protected:
    void render(const Jsoner::Object &object) override;
    void extract(Jsoner::Object &object) const override;
    void makeVisible(bool visibible, int result = Success) override;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAEDITDIALOG_H
