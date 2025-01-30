#ifndef WIDGETRY_DATAINTERFACEBASE_P_H
#define WIDGETRY_DATAINTERFACEBASE_P_H

#include "datainterfacebase.h"

namespace Widgetry {

class DataInterfaceBasePrivate
{
public:
    Jsoner::TableModel *model = nullptr;
    QMenu *contextMenu = nullptr;
};

}

#endif // WIDGETRY_DATAINTERFACEBASE_P_H
