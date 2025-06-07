#ifndef LOGINEDIT_H
#define LOGINEDIT_H

#include <Widgetry/global.h>
#include <Widgetry/lineedit.h>

namespace Widgetry {

class WIDGETRY_EXPORT LoginEdit : public Widgetry::LineEdit
{
    Q_OBJECT

public:
    enum LoginType {
        UsernameLogin,
        EmailLogin
    };

    explicit LoginEdit(QWidget *parent = nullptr);
    virtual ~LoginEdit();

    LoginType loginType() const;
    void setLoginType(LoginType type);
};

} // namespace Widgetry

#endif // LOGINEDIT_H
