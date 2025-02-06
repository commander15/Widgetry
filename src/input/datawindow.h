#ifndef WIDGETRY_DATAWINDOW_H
#define WIDGETRY_DATAWINDOW_H

#include <Widgetry/global.h>
#include <Widgetry/userinterface.h>
#include <Widgetry/abstractdataedit.h>

namespace Widgetry {

class WIDGETRY_EXPORT DataWindow : public UserInterface, public AbstractDataEdit
{
    Q_OBJECT

public:
    DataWindow(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataWindow();

    QWidget *editWidget() const override;
    EditType editType() const override;

public slots:
    void show();
    void show(const Jsoner::Object &object);
    void add(const Jsoner::Object &object);
    void edit(const Jsoner::Object &object);
    void clear() override;

signals:
    void editingFinished(int result = Accepted);

protected:
    void render(const Jsoner::Object &object, Operation operation) override;
    void extract(Jsoner::Object &object, Operation operation) const override;
    bool validateInput() override;
    void makeWriteable(bool writeable) override;

    void registerEdit(AbstractDataEdit *edit);
    void registerEdit(const QString &field, AbstractDataEdit *edit);

private:
    using UserInterface::d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAWINDOW_H
