#ifndef WIDGETRY_DATAWINDOW_H
#define WIDGETRY_DATAWINDOW_H

#include <Widgetry/global.h>
#include <Widgetry/dataedit.h>

namespace Widgetry {

class WIDGETRY_EXPORT DataWindow : public DataEdit
{
    Q_OBJECT

public:
    DataWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    DataWindow(const QByteArray &id, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~DataWindow();

    void registerAdd(const DataEditFinishedCallback &callback);
    void registerEdit(const DataEditFinishedCallback &callback);

    EditType editType() const override final
    { return WindowEdit; }

public slots:
    void begin() override;
    void end(int result = Success) override;

protected:
    void render(const Jsoner::Object &object) override;
    void extract(Jsoner::Object &object) const override;
    bool validateInput() override;
    void makeWriteable(bool writeable) override;

    void registerEdit(AbstractDataEdit *edit);
    void registerEdit(const QString &field, AbstractDataEdit *edit);
};

} // namespace Widgetry

#endif // WIDGETRY_DATAWINDOW_H
