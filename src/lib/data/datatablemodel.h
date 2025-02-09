#ifndef WIDGETRY_DATATABLEMODEL_H
#define WIDGETRY_DATATABLEMODEL_H

#include <Widgetry/global.h>

#include <Jsoner/tablemodel.h>

namespace Widgetry {

class DataTableModelPrivate;
class WIDGETRY_EXPORT DataTableModel : public Jsoner::TableModel
{
    Q_OBJECT

public:
    explicit DataTableModel(QObject *parent = nullptr);
    virtual ~DataTableModel();

    QString sortField() const;
    Qt::SortOrder sortOrder() const;
    void sort(int column, Qt::SortOrder order) override;

signals:
    void fetchRequested();

private:
    QScopedPointer<DataTableModelPrivate> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DATATABLEMODEL_H
