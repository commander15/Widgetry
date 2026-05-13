#ifndef WIDGETRY_DATAEDIT_H
#define WIDGETRY_DATAEDIT_H

#include <Widgetry/global.h>
#include <Widgetry/widget.h>
#include <Widgetry/abstractdataedit.h>

#include <Jsoner/object.h>

namespace Widgetry {

class DataEditPrivate;
class WIDGETRY_EXPORT DataEdit : public Widget, public AbstractDataEdit
{
    Q_OBJECT

public:
    DataEdit(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataEdit();

    EditType editType() const override
    { return WidgetEdit; }

public slots:
    void begin() override { AbstractDataEdit::begin(); }
    void end(int result = Success) override { AbstractDataEdit::end(result); }
    void reset() override { AbstractDataEdit::reset(); }
    void clear() override { AbstractDataEdit::clear(); }

signals:
    void completeChanged();
    void editingFinished(int result = Success);

protected:
    DataEdit(DataEditPrivate *d, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    void render(const Jsoner::Object &object) override;
    void extract(Jsoner::Object &object) const override;
    void makeVisible(bool visible, int result = Success) override;

    using Widget::d_ptr;

    friend class DataEditDialogHelper;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAEDIT_H
