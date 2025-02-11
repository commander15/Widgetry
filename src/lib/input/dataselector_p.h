#ifndef WIDGETRY_DATASELECTOR_P_H
#define WIDGETRY_DATASELECTOR_P_H

#include "dataselector.h"

#include <Widgetry/datatablemodel.h>
#include <Widgetry/dataquery.h>

namespace Widgetry {

class DataSelectorPrivate
{
public:
    DataTableModel model;

    int dataSearchThreshold = 3;
    DataQuery dataQuery;
    AbstractDataController *dataController = nullptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DATASELECTOR_P_H
