#include "loginedit.h"

#include <Widgetry/private/lineedit_p.h>

#include <QtCore/qcoreapplication.h>

namespace Widgetry {

class LoginEditPrivate : public LineEditPrivate
{
    Q_DECLARE_TR_FUNCTIONS(Widgetry::LoginEdit)

public:
    LoginEditPrivate(LoginEdit *q)
        : LineEditPrivate(q)
        , iconAction(nullptr)
        , loginType(LoginEdit::UsernameLogin)
    {}

    void initEdit() override
    {
        iconAction = new QAction(QIcon(":/widgetry/icons/login_icon.png"), QString(), q_ptr);
        validator = new QRegularExpressionValidator(expressions.at(loginType), q_ptr);
    }

    void retranslateEdit() override
    {
        switch (loginType) {
        case LoginEdit::UsernameLogin:
            q_ptr->setPlaceholderText(tr("username"));
            break;

        case LoginEdit::EmailLogin:
            q_ptr->setPlaceholderText(tr("email"));
            break;
        }
    }

    QAction *iconAction;
    LoginEdit::LoginType loginType;

    static inline const QList<QRegularExpression> expressions = {
        // Regular expression for username login
        QRegularExpression(R"(^[a-zA-Z0-9_]{3,16}$)"),
        // Regular expression for email
        QRegularExpression(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)")
    };
};

LoginEdit::LoginEdit(QWidget *parent)
    : LineEdit(new LoginEditPrivate(this), parent)
{
    WIDGETRY_D(LoginEdit);
    setClearButtonEnabled(true);
    addAction(d->iconAction, LeadingPosition);
}

LoginEdit::~LoginEdit()
{
}

LoginEdit::LoginType LoginEdit::loginType() const
{
    WIDGETRY_D(const LoginEdit);
    return d->loginType;
}

void LoginEdit::setLoginType(LoginType type)
{
    WIDGETRY_D(LoginEdit);
    if (d->loginType != type) {
        d->loginType = type;
        d->validator->setRegularExpression(d->expressions.at(type));
        d->retranslateEdit();
    }
}

} // namespace Widgetry
