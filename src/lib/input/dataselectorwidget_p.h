#ifndef WIDGETRY_DATASELECTORWIDGET_P_H
#define WIDGETRY_DATASELECTORWIDGET_P_H

#include "dataselectorwidget.h"

#include <DataGate/tablemodel.h>
#include <DataGate/dataquery.h>

namespace Widgetry {

class DataSelectorWidgetPrivate
{
public:
    DataGate::TableModel model;

    int dataSearchThreshold = 3;
    DataGate::DataQuery dataQuery;
    DataGate::AbstractDataController *dataController = nullptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DATASELECTORWIDGET_P_H
