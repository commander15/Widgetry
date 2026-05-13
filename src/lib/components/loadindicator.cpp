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
    { if (!movie) movie.reset(new QMovie(":/widgetry/animations/busy_indicator.gif"));  }

    bool isPlaying() const
    { return m_playing; }

    void start()
    {
        s_playRef.ref();
        if (movie->state() != QMovie::Running)
            movie->start();
        m_playing = true;
    }

    void stop()
    {
        if (!s_playRef.deref() && movie->state() == QMovie::Running)
            movie->stop();
        m_playing = false;
    }

    QLabel *label = nullptr;
    static inline QScopedPointer<QMovie> movie = QScopedPointer<QMovie>(nullptr);

private:
    bool m_playing = false;

    static inline QAtomicInt s_playRef;
};

LoadIndicator::LoadIndicator(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new LoadIndicatorPrivate())
{
    d_ptr->label = new QLabel(this);
    d_ptr->label->setMovie(d_ptr->movie.get());
    d_ptr->label->setAlignment(Qt::AlignCenter);
    d_ptr->label->setScaledContents(true);

    setMinimumSize(16, 16);
    setMaximumSize(200, 200);
}

LoadIndicator::~LoadIndicator()
{
    if (d_ptr->isPlaying())
        d_ptr->stop();
}

void LoadIndicator::showEvent(QShowEvent *event)
{
    d_ptr->start();
    QWidget::showEvent(event);
}

void LoadIndicator::hideEvent(QHideEvent *event)
{
    d_ptr->stop();
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
