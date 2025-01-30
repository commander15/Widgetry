#ifndef WIDGETRY_DATAINTERFACE_P_H
#define WIDGETRY_DATAINTERFACE_P_H

#include "datainterface.h"

#include <Widgetry/private/userinterface_p.h>
#include <Widgetry/private/datainterfacebase_p.h>

namespace Widgetry {

class DataInterfacePrivate : public UserInterfacePrivate, public DataInterfaceBasePrivate
{
public:
    DataInterfacePrivate(const QByteArray &id, DataInterface *q);
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACE_P_H
