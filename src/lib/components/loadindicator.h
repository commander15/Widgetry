#ifndef WIDGETRY_LOADINDICATOR_H
#define WIDGETRY_LOADINDICATOR_H

#include <Widgetry/global.h>

#include <QtWidgets/qwidget.h>

namespace Widgetry {

class LoadIndicatorPrivate;
class WIDGETRY_EXPORT LoadIndicator : public QWidget
{
    Q_OBJECT

public:
    explicit LoadIndicator(QWidget *parent = nullptr);
    virtual ~LoadIndicator();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QScopedPointer<LoadIndicatorPrivate> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_LOADINDICATOR_H
