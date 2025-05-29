#include "searchbar.h"

#include <QtGui/qevent.h>

namespace Widgetry {

SearchBar::SearchBar(QWidget *parent)
    : QLineEdit(parent)
    , m_iconAction(new QAction(QIcon(":/widgetry/icons/search_icon.png"), QString(), this))
    , m_goAction(new QAction(QIcon(":/widgetry/icons/search_go.png"), QString(), this))
{
    addAction(m_iconAction, LeadingPosition);
    addAction(m_goAction, TrailingPosition);
    connect(this, &QLineEdit::returnPressed, this, &SearchBar::searchRequested);
}

void SearchBar::retranslateUi()
{
    m_iconAction->setToolTip(tr("Search"));
    setPlaceholderText(tr("Search..."));
}

void SearchBar::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
        event->accept();
        return;
    }

    QLineEdit::changeEvent(event);
}

} // namespace Widgetry
