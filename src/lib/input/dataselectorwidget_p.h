#ifndef WIDGETRY_DATASELECTORWIDGET_P_H
#define WIDGETRY_DATASELECTORWIDGET_P_H

#include "dataselectorwidget.h"

#include <Widgetry/datatablemodel.h>
#include <Widgetry/dataquery.h>

namespace Widgetry {

class DataSelectorWidgetPrivate
{
public:
    DataTableModel model;

    int dataSearchThreshold = 3;
    DataQuery dataQuery;
    AbstractDataController *dataController = nullptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DATASELECTORWIDGET_P_H
