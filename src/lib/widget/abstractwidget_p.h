#ifndef WIDGETRY_ABSTRACTWIDGET_P_H
#define WIDGETRY_ABSTRACTWIDGET_P_H

#include "abstractwidget.h"

#include <QtCore/qbytearray.h>

#include <QtGui/qaction.h>

namespace Widgetry {

class AbstractWidgetPrivate
{
public:
    explicit AbstractWidgetPrivate(AbstractWidget *q, QWidget *widget);
    virtual ~AbstractWidgetPrivate();

    template <typename Widget> void initWidget();

    void updateLayout();
    void updateLayout(bool window);

    virtual bool processEvent(QEvent *event);

    AbstractWidget *q_ptr;

    QByteArray id;
    QAction *action;
    QWidget *widget;

private:
    bool firstShow;
};

template<typename Widget>
inline void AbstractWidgetPrivate::initWidget()
{
    const QString className(Widget::staticMetaObject.className());

    Widget *widget = static_cast<Widget *>(q_ptr);
    widget->setIcon(QIcon(":/widgetry/icons/widgetry.png"));
    widget->setTitle(className.section("::", -1));

    QObject::connect(widget, &QWidget::windowIconChanged, widget, &Widget::iconChanged);
    QObject::connect(widget, &QWidget::windowTitleChanged, widget, &Widget::titleChanged);
}

}

#endif // WIDGETRY_ABSTRACTWIDGET_P_H
