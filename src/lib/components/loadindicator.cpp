#include "loadindicator.h"

#include <QtGui/qmovie.h>
#include <QtGui/qevent.h>

#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>

namespace Widgetry {

class LoadIndicatorPrivate
{
public:
    LoadIndicatorPrivate()
    { if (!movie) movie = new QMovie(":/widgetry/animations/busy_indicator.gif"); movieRef.ref();  }

    ~LoadIndicatorPrivate()
    { if (movieRef.deref()) return; delete movie; movie = nullptr; }

    bool playing = false;
    QLabel *label = nullptr;

    static inline QAtomicInt playRef;
    static inline QAtomicInt movieRef;
    static inline QMovie *movie = nullptr;
};

LoadIndicator::LoadIndicator(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new LoadIndicatorPrivate())
{
    d_ptr->label = new QLabel(this);
    d_ptr->label->setMovie(d_ptr->movie);
    d_ptr->label->setAlignment(Qt::AlignCenter);
    d_ptr->label->setScaledContents(true);

    setMinimumSize(16, 16);
    setMaximumSize(200, 200);
}

LoadIndicator::~LoadIndicator()
{
    if (d_ptr->playing)
        d_ptr->playRef.deref();
}

void LoadIndicator::showEvent(QShowEvent *event)
{
    d_ptr->playRef.ref();
    if (d_ptr->movie->state() != QMovie::Running)
        d_ptr->movie->start();
    QWidget::showEvent(event);
}

void LoadIndicator::hideEvent(QHideEvent *event)
{
    if (!d_ptr->playRef.deref() && d_ptr->movie->state() == QMovie::Running)
        d_ptr->movie->stop();
    QWidget::hideEvent(event);
}

void LoadIndicator::resizeEvent(QResizeEvent *event)
{
    static const QSize gifSize(200, 200);
    const QSize targetSize = gifSize.scaled(event->size(), Qt::KeepAspectRatio);
    const QSize targetPos((event->size() - targetSize) / 2);

    d_ptr->label->setGeometry(targetPos.width(), targetPos.height(), targetSize.width(), targetSize.height());
    QWidget::resizeEvent(event);
}

} // namespace Widgetry
