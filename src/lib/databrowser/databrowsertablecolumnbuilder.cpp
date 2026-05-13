#include "databrowsertablecolumnbuilder.h"

#include <Jsoner/tablemodel.h>

namespace Widgetry {

class DataBrowserTableColumnBuilderData : public QSharedData
{
public:
    QString label;
    QString field;
    int occurence = 0;

    int size = -1;

    QHeaderView::ResizeMode resizeMode = QHeaderView::Stretch;
    bool useResize = false;

    bool visible = true;
    bool useVisibility = false;
};

DataBrowserTableColumnBuilder::DataBrowserTableColumnBuilder(const QString &field, int occurence)
    : d_ptr(new DataBrowserTableColumnBuilderData())
{
    d_ptr->field = field;
    d_ptr->occurence = occurence;
}

DataBrowserTableColumnBuilder &DataBrowserTableColumnBuilder::operator=(const DataBrowserTableColumnBuilder &other)
{
    if (this != &other)
        d_ptr = other.d_ptr;
    return *this;
}

DataBrowserTableColumnBuilder::DataBrowserTableColumnBuilder(const DataBrowserTableColumnBuilder &other) = default;

DataBrowserTableColumnBuilder::~DataBrowserTableColumnBuilder() = default;

int DataBrowserTableColumnBuilder::index(const Jsoner::TableModel *model) const
{
    const QStringList fields = model->fields();

    int index = fields.indexOf(d_ptr->field);
    if (d_ptr->occurence == 0)
        return index;

    int occurence = d_ptr->occurence;
    for (int i(occurence); i > 0 && index >= 0; --i)
        index = fields.indexOf(d_ptr->field, index + 1);
    return index;
}

QString DataBrowserTableColumnBuilder::fieldName() const
{
    return d_ptr->field;
}

DataBrowserTableColumnBuilder &DataBrowserTableColumnBuilder::label(const QString &label)
{
    d_ptr->label = label;
    return *this;
}

DataBrowserTableColumnBuilder &DataBrowserTableColumnBuilder::field(const QString &field)
{
    d_ptr->field = field;
    return *this;
}

DataBrowserTableColumnBuilder &DataBrowserTableColumnBuilder::size(int size)
{
    d_ptr->size = size;
    return *this;
}

DataBrowserTableColumnBuilder &DataBrowserTableColumnBuilder::resize(QHeaderView::ResizeMode mode)
{
    d_ptr->resizeMode = mode;
    d_ptr->useResize = true;
    return *this;
}

DataBrowserTableColumnBuilder &DataBrowserTableColumnBuilder::show()
{
    d_ptr->visible = true;
    d_ptr->useVisibility = true;
    return *this;
}

DataBrowserTableColumnBuilder &DataBrowserTableColumnBuilder::hide()
{
    d_ptr->visible = false;
    d_ptr->useVisibility = true;
    return *this;
}

DataBrowserTableColumnBuilder &DataBrowserTableColumnBuilder::visible(bool visible)
{
    d_ptr->visible = visible;
    d_ptr->useVisibility = true;
    return *this;
}

void DataBrowserTableColumnBuilder::build(QHeaderView *header, Jsoner::TableModel *model) const
{
    int index = this->index(model);
    if (index < 0)
        return;

    if (!d_ptr->label.isEmpty())
        model->setHeaderData(index, Qt::Horizontal, d_ptr->label);

    if (d_ptr->useResize)
        header->setSectionResizeMode(index, d_ptr->resizeMode);

    if (d_ptr->size > 0)
        header->resizeSection(index, d_ptr->size);

    if (d_ptr->useVisibility)
        header->setSectionHidden(index, !d_ptr->visible);
}

} // namespace Widgetry
