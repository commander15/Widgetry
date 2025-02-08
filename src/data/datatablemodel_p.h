#ifndef WIDGETRY_DATATABLEMODEL_P_H
#define WIDGETRY_DATATABLEMODEL_P_H

#include "datatablemodel.h"

namespace Widgetry {

class DataTableModelPrivate
{
public:
    int sortColumn = -1;
    Qt::SortOrder sortOrder = Qt::DescendingOrder;
};

} // namespace Widgetry

#endif // WIDGETRY_DATATABLEMODEL_P_H
