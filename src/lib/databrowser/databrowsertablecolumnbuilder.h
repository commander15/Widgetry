#ifndef WIDGETRY_DATABROWSERTABLECOLUMNBUILDER_H
#define WIDGETRY_DATABROWSERTABLECOLUMNBUILDER_H

#include <Widgetry/global.h>

#include <QtCore/qshareddata.h>

#include <QtWidgets/qheaderview.h>

namespace Jsoner {
class TableModel;
}

namespace Widgetry {

class DataBrowserTableColumnBuilderData;
class WIDGETRY_EXPORT DataBrowserTableColumnBuilder
{
public:
    DataBrowserTableColumnBuilder(const QString &field);
    DataBrowserTableColumnBuilder(const DataBrowserTableColumnBuilder &other);
    ~DataBrowserTableColumnBuilder();

    DataBrowserTableColumnBuilder &operator=(const DataBrowserTableColumnBuilder &other);

    int index(const Jsoner::TableModel *model) const;
    QString fieldName() const;

    DataBrowserTableColumnBuilder &label(const QString &label);
    DataBrowserTableColumnBuilder &field(const QString &field);

    DataBrowserTableColumnBuilder &size(int size);
    DataBrowserTableColumnBuilder &resize(QHeaderView::ResizeMode mode);

    DataBrowserTableColumnBuilder &show();
    DataBrowserTableColumnBuilder &hide();
    DataBrowserTableColumnBuilder &visible(bool visible);

    void build(QHeaderView *header, Jsoner::TableModel *model) const;

private:
    QExplicitlySharedDataPointer<DataBrowserTableColumnBuilderData> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_DATABROWSERTABLECOLUMNBUILDER_H
