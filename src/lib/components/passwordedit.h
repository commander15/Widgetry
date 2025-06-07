#ifndef PASSWORDEDIT_H
#define PASSWORDEDIT_H

#include <Widgetry/global.h>
#include <Widgetry/lineedit.h>

namespace Widgetry {

class WIDGETRY_EXPORT PasswordEdit : public Widgetry::LineEdit
{
    Q_OBJECT

public:
    explicit PasswordEdit(QWidget *parent = nullptr);
    virtual ~PasswordEdit();

private:
    Q_SLOT void togleEchoMode();
};

} // namespace Widgetry

#endif // PASSWORDEDIT_H
