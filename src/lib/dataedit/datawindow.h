#ifndef WIDGETRY_DATAWINDOW_H
#define WIDGETRY_DATAWINDOW_H

#include <Widgetry/global.h>
#include <Widgetry/widget.h>
#include <Widgetry/abstractdataedit.h>

namespace Widgetry {

class WIDGETRY_EXPORT DataWindow : public Widget, public AbstractDataEdit
{
    Q_OBJECT

public:
    DataWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    DataWindow(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataWindow();

    void registerAdd(const DataEditFinishedCallback &callback);
    void registerEdit(const DataEditFinishedCallback &callback);

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
    void editingFinished(int result = Accepted);

protected:
    void render(const Jsoner::Object &object, Operation operation) override;
    void extract(Jsoner::Object &object, Operation operation) const override;
    bool validateInput() override;
    void makeWriteable(bool writeable) override;

    void finishEditing(int result = Accepted) override;

    void registerEdit(AbstractDataEdit *edit);
    void registerEdit(const QString &field, AbstractDataEdit *edit);

private:
    using Widget::d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAWINDOW_H
