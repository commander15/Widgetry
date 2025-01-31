#ifndef WIDGETRY_DATAINTERFACE_P_H
#define WIDGETRY_DATAINTERFACE_P_H

#include "datainterface.h"

#include <Widgetry/private/userinterface_p.h>
#include <Widgetry/private/datainterfaceforge_p.h>

namespace Widgetry {

class DataInterfacePrivate : public UserInterfacePrivate, public DataInterfaceForgePrivate
{
public:
    DataInterfacePrivate(DataInterface *q, const QByteArray &id);
    ~DataInterfacePrivate();

    inline DataInterface *forgeInterface() const override
    { WIDGETRY_Q(DataInterface); return q; }

    DataInterfaceForge *forge;
    AbstractDataController *dataController;
};

} // namespace Widgetry

#endif // WIDGETRY_DATAINTERFACE_P_H
