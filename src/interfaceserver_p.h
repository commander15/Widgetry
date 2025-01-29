#ifndef SYSTEMUS_INTERFACESERVER_P_H
#define SYSTEMUS_INTERFACESERVER_P_H

#include <Widgetry/interfaceserver.h>

namespace Widgetry {

class InterfaceServerPrivate
{
public:
    InterfaceServerPrivate(InterfaceServer *q);

    InterfaceServer *q;

    UserInterface *currentInterface;
};

}

#endif // SYSTEMUS_INTERFACESERVER_P_H
