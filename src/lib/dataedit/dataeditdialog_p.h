#ifndef WIDGETRY_DATAEDITDIALOG_P_H
#define WIDGETRY_DATAEDITDIALOG_P_H

#include <Widgetry/abstractdataedit.h>

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qdialogbuttonbox.h>

namespace Widgetry {

class DataEditDialogHelper : public QDialog, public AbstractDataEdit
{
    Q_OBJECT

public:
    DataEditDialogHelper(QWidget *parent, Qt::WindowFlags flags, bool multiple = false);

    void init(AbstractDataEdit *edit);

    void updateErrorState(bool show);
    void updateButtonStates(bool saveable);

    DataBrowser *browser() const override;
    void setBrowser(DataBrowser *interface) override;

    void exec(const DataEditFinishedCallback &callback) override;

    QWidget *editWidget() const override;
    EditType editType() const override;

public slots:
    void updateError() { updateErrorState(true); }
    void updateButtons() { updateButtonStates(false); }

    void setVisible(bool v) override;

    void clear() override;

protected:
    void render(const Jsoner::Object &object, Operation operation) override;
    void extract(Jsoner::Object &object, Operation operation) const override;
    bool validateInput() override;
    void makeWriteable(bool writeable) override;

    using AbstractDataEdit::d_ptr;

private:
    AbstractDataEdit *m_edit;
    QLabel *m_errorOutput;
    QDialogButtonBox *m_buttonBox;

    bool m_multiple = false;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAEDITDIALOG_P_H
