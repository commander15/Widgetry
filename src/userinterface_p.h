#ifndef WIDGETRY_USERINTERFACE_P_H
#define WIDGETRY_USERINTERFACE_P_H

#include "userinterface.h"

namespace Widgetry {

class UserInterfacePrivate
{
public:
    UserInterfacePrivate(UserInterface *q);
    UserInterfacePrivate(const QByteArray &id, UserInterface *q);

    UserInterface *q_ptr;

    const QByteArray id;
    QAction *action;
};

} // namespace Widgetry

#endif // WIDGETRY_USERINTERFACE_P_H
