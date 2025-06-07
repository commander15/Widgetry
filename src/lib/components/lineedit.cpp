#include "lineedit.h"
#include "lineedit_p.h"

#include <QtGui/qevent.h>

namespace Widgetry {

LineEdit::LineEdit(QWidget *parent)
    : LineEdit(new LineEditPrivate(this), parent)
{
}

LineEdit::LineEdit(LineEditPrivate *d, QWidget *parent)
    : QLineEdit(parent)
    , d_ptr(d)
{
    d->initEdit();
    if (d->validator)
        setValidator(d->validator);
    d->retranslateEdit();
}

LineEdit::~LineEdit()
{
}

QRegularExpression LineEdit::regularExpression() const
{
    return (d_ptr->validator ? d_ptr->validator->regularExpression() : QRegularExpression());
}

void LineEdit::setRegularExpression(const QRegularExpression &regex)
{
    if (d_ptr->validator) {
        d_ptr->validator->setRegularExpression(regex);
    } else {
        d_ptr->validator = new QRegularExpressionValidator(regex, this);
        setValidator(d_ptr->validator);
    }
}

void LineEdit::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        d_ptr->retranslateEdit();
        event->accept();
    }

    QLineEdit::changeEvent(event);
}

LineEditPrivate::LineEditPrivate(LineEdit *q)
    : q_ptr(q)
    , validator(nullptr)
{
}

} // namespace Widgetry
