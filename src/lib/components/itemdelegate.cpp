#include "itemdelegate.h"

#include <QtWidgets/qapplication.h>

#include <Jsoner/object.h>
#include <Jsoner/tablemodel.h>
#include <Jsoner/itemmodel.h>

namespace Widgetry {

ItemDelegate::ItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , m_painter(nullptr)
    , m_option(nullptr)
    , m_widget(parent != nullptr && parent->isWidgetType() ? static_cast<QWidget *>(parent) : nullptr)
{
}

ItemDelegate::~ItemDelegate()
{    
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ItemDelegate *delegate = const_cast<ItemDelegate *>(this);
    delegate->preparePaint(painter, option, index);
    delegate->render(index);
}

QString ItemDelegate::fieldName(const QModelIndex &index, int *occurence) const
{
    const Jsoner::ItemModel *itemModel = this->model(index);
    if (itemModel == nullptr || !itemModel->inherits("Jsoner::TableModel"))
        return QString();

    const Jsoner::TableModel *model = static_cast<const Jsoner::TableModel *>(itemModel);
    const QStringList fields = model->fields();
    const QString field = fields.at(index.column());

    if (occurence == nullptr)
        return field;

    *occurence = std::count_if(fields.begin(), fields.begin() + index.column() + 1, [&field](const QString &one) {
        return field == one;
    }) - 1;

    return field;
}

Jsoner::Object ItemDelegate::object(const QModelIndex &index) const
{
    const Jsoner::ItemModel *model = this->model(index);
    return (model != nullptr ? model->object(index.row()) : Jsoner::Object());
}

const Jsoner::ItemModel *ItemDelegate::model(const QModelIndex &index) const
{
    const QAbstractItemModel *model = index.model();
    if (model != nullptr && model->inherits("Jsoner::ItemModel"))
        return static_cast<const Jsoner::ItemModel *>(model);
    else
        return nullptr;
}

void ItemDelegate::initOption(QStyleOption &option)
{
    option = *m_option;
}

void ItemDelegate::render(const QModelIndex &index)
{
    QStyledItemDelegate::paint(m_painter, *static_cast<const QStyleOptionViewItem *>(m_option), index);
}

void ItemDelegate::draw(ControlElement element, const QStyleOption &option)
{
    if (m_painter != nullptr)
        qApp->style()->drawControl(element, &option, m_painter, m_widget);
}

void ItemDelegate::draw(ComplexControl control, const QStyleOptionComplex &option)
{
    if (m_painter != nullptr)
        qApp->style()->drawComplexControl(control, &option, m_painter, m_widget);
}

void ItemDelegate::preparePaint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    m_painter = painter;
    m_option = &option;
    Q_UNUSED(index);
}

} // namespace Widgetry
