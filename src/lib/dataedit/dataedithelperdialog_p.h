#ifndef WIDGETRY_DATAEDITHELPERDIALOG_P_H
#define WIDGETRY_DATAEDITHELPERDIALOG_P_H

#include <Widgetry/dataeditdialog.h>

#include <QtCore/qtimer.h>

#include <QtWidgets/qlabel.h>
#include <QtWidgets/qdialogbuttonbox.h>

namespace Widgetry {

class DataEditDialogHelper : public DataEditDialog
{
    Q_OBJECT

public:
    DataEditDialogHelper(QWidget *parent, Qt::WindowFlags flags, bool multiple = false);

    void init(AbstractDataEdit *edit);
    AbstractDataEdit *wrappedEdit() const { return m_edit; }

    void updateErrorState(bool show);
    void updateButtonStates(bool saveable);

    void run(const DataEditFinishedCallback &callback) override;

public slots:
    void checkInput();
    void clear() override;

protected:
    void render(const Jsoner::Object &object) override;
    void extract(Jsoner::Object &object) const override;
    bool validateInput() override;
    void makeWriteable(bool writable) override;

    using AbstractDataEdit::d_ptr;

private:
    AbstractDataEdit *m_edit;
    QLabel *m_errorOutput;
    QTimer m_errorCleanTimer;
    QDialogButtonBox *m_buttonBox;

    bool m_multiple = false;
    DataEditFinishedCallback m_callback;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAEDITHELPERDIALOG_P_H
