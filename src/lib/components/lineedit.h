#ifndef WIDGETRY_LINEEDIT_H
#define WIDGETRY_LINEEDIT_H

#include <Widgetry/global.h>

#include <QtWidgets/qlineedit.h>

namespace Widgetry {

class LineEditPrivate;
class WIDGETRY_EXPORT LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit LineEdit(QWidget *parent = nullptr);
    virtual ~LineEdit();

    QRegularExpression regularExpression() const;
    void setRegularExpression(const QRegularExpression &regex);

protected:
    LineEdit(LineEditPrivate *d, QWidget *parent = nullptr);

    void changeEvent(QEvent *event) override;

    QScopedPointer<LineEditPrivate> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_LINEEDIT_H
