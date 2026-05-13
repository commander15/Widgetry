#ifndef WIDGETRY_ITEMDELEGATE_H
#define WIDGETRY_ITEMDELEGATE_H

#include <Widgetry/global.h>

#include <QtWidgets/qstyleditemdelegate.h>

namespace Jsoner {
class Object;
class ItemModel;
}

namespace Widgetry {

class WIDGETRY_EXPORT ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    using ControlElement = QStyle::ControlElement;
    using ComplexControl = QStyle::ComplexControl;

    explicit ItemDelegate(QObject *parent = nullptr);
    virtual ~ItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QString fieldName(const QModelIndex &index, int *occurence = nullptr) const;
    Jsoner::Object object(const QModelIndex &index) const;
    const Jsoner::ItemModel *model(const QModelIndex &index) const;

protected:
    void initOption(QStyleOption &option);

    virtual void render(const QModelIndex &index);
    void draw(ControlElement element, const QStyleOption &option);
    void draw(ComplexControl control, const QStyleOptionComplex &option);

private:
    void preparePaint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index);

    QPainter *m_painter;
    const QStyleOption *m_option;
    QWidget *m_widget;
};

} // namespace Widgetry

#endif // WIDGETRY_ITEMDELEGATE_H
