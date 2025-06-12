#include "searchbar.h"

#include <Widgetry/private/lineedit_p.h>

#include <QtCore/qcoreapplication.h>

#include <QtGui/qshortcut.h>

namespace Widgetry {

class SearchBarPrivate : public LineEditPrivate
{
    Q_DECLARE_TR_FUNCTIONS(Widgetry::SearchBar)

public:
    SearchBarPrivate(SearchBar *q)
        : LineEditPrivate(q)
    {}

    void initEdit() override
    {
        WIDGETRY_Q(SearchBar);

        iconAction = new QAction(QIcon(":/widgetry/icons/search_icon.png"), QString(), q);
        q_ptr->addAction(iconAction, QLineEdit::LeadingPosition);

        goAction = new QAction(QIcon(":/widgetry/icons/search_go.png"), QString(), q);
        QObject::connect(goAction, &QAction::triggered, q, &SearchBar::searchRequested);
        q_ptr->addAction(goAction, QLineEdit::TrailingPosition);

        QObject::connect(q, &QLineEdit::returnPressed, q, &SearchBar::searchRequested);
        QObject::connect(q, &QLineEdit::textEdited, q, &SearchBar::completionsRequested);
    }

    void retranslateEdit() override
    {
        q_ptr->setPlaceholderText(tr("Search..."));
    }

    void createCompleter() override
    {
        WIDGETRY_Q(SearchBar);
        LineEditPrivate::createCompleter();

        QCompleter *completer = this->completer();
        QObject::connect(completer, QOverload<const QString &>::of(&QCompleter::activated), q, [q](const QString &) {
            emit q->searchRequested();
        });
    }

    QAction *iconAction;
    QAction *goAction;
};

SearchBar::SearchBar(QWidget *parent)
    : LineEdit(new SearchBarPrivate(this), parent)
{
    QShortcut *suggest = new QShortcut(QKeySequence(Qt::Key_Control | Qt::Key_Space), this);
    connect(suggest, &QShortcut::activated, this, [this] { emit completionsRequested(text()); });
}

SearchBar::~SearchBar()
{
}

} // namespace Widgetry
