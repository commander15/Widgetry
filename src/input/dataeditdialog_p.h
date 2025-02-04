#ifndef WIDGETRY_DATAEDITDIALOG_P_H
#define WIDGETRY_DATAEDITDIALOG_P_H

#include <Widgetry/abstractdataedit.h>

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qdialogbuttonbox.h>

namespace Widgetry {

class DataEditDialogHelper : public QDialog
{
    Q_OBJECT

public:
    DataEditDialogHelper(QWidget *parent, Qt::WindowFlags flags);

    void init(AbstractDataEdit *edit);

    void updateErrorState(bool show);
    void updateButtonStates(bool saveable);

public slots:
    void updateError() { updateErrorState(true); }
    void updateButtons() { updateButtonStates(false); }

    void setVisible(bool v) override;

private:
    AbstractDataEdit *m_edit;
    QLabel *m_errorOutput;
    QDialogButtonBox *m_buttonBox;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAEDITDIALOG_P_H
