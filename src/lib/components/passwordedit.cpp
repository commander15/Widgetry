#include "passwordedit.h"

#include <Widgetry/private/lineedit_p.h>

#include <QtCore/qcoreapplication.h>

namespace Widgetry {

class PasswordEditPrivate : public LineEditPrivate
{
    Q_DECLARE_TR_FUNCTIONS(Widgetry::PasswordEdit)

public:
    PasswordEditPrivate(PasswordEdit *q)
        : LineEditPrivate(q)
    {
    }

    void initEdit() override
    {
        iconAction = new QAction(QIcon(":/widgetry/icons/login_password.png"), QString(), q_ptr);
        showPassword = new QAction(QIcon(":/widgetry/icons/password_show.png"), QString(), q_ptr);

        static const QRegularExpression regex(R"(^.{6,}$)");
        validator = new QRegularExpressionValidator(regex, q_ptr);
    }

    void retranslateEdit() override
    {
        q_ptr->setPlaceholderText(tr("password"));
    }

    QAction *iconAction;
    QAction *showPassword;
};

PasswordEdit::PasswordEdit(QWidget *parent)
    : LineEdit(new PasswordEditPrivate(this), parent)
{
    WIDGETRY_D(PasswordEdit);

    setEchoMode(QLineEdit::Password);

    addAction(d->iconAction, LeadingPosition);

    connect(d->showPassword, &QAction::triggered, this, &PasswordEdit::togleEchoMode);
    addAction(d->showPassword, TrailingPosition);
}

PasswordEdit::~PasswordEdit()
{
}

void PasswordEdit::togleEchoMode()
{
    WIDGETRY_D(PasswordEdit);

    if (echoMode() == Password) {
        setEchoMode(QLineEdit::Normal);
        d->showPassword->setIcon(QIcon(":/widgetry/icons/password_hide.png"));
    }  else {
        setEchoMode(QLineEdit::Password);
        d->showPassword->setIcon(QIcon(":/widgetry/icons/password_show.png"));
    }

    d->retranslateEdit();
}

} // namespace Widgetry
