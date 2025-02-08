#include "datatablemodel.h"
#include "datatablemodel_p.h"

namespace Widgetry {

DataTableModel::DataTableModel(QObject *parent)
    : Jsoner::TableModel{parent}
    , d_ptr(new DataTableModelPrivate())
{
}

QString DataTableModel::sortField() const
{
    return d_ptr->sortField;
}

Qt::SortOrder DataTableModel::sortOrder() const
{
    return d_ptr->sortOrder;
}

DataTableModel::~DataTableModel() = default;

void DataTableModel::sort(int column, Qt::SortOrder order)
{
    d_ptr->sortField = fieldName(column);
    d_ptr->sortOrder = order;
    emit fetchRequested();
}

} // namespace Widgetry
