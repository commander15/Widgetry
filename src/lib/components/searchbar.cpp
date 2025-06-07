#include "searchbar.h"

#include <Widgetry/private/lineedit_p.h>

#include <QtCore/qcoreapplication.h>
#include <QtCore/qstringlistmodel.h>

#include <QtWidgets/qcompleter.h>

namespace Widgetry {

class SearchBarPrivate : public LineEditPrivate
{
    Q_DECLARE_TR_FUNCTIONS(Widgetry::SearchBar)

public:
    SearchBarPrivate(SearchBar *q)
        : LineEditPrivate(q)
        , m_completionModel(nullptr)
        , m_completer(nullptr)
    {}

    void initEdit() override
    {
        iconAction = new QAction(QIcon(":/widgetry/icons/search_icon.png"), QString(), q_ptr);
        goAction = new QAction(QIcon(":/widgetry/icons/search_go.png"), QString(), q_ptr);
    }

    void retranslateEdit() override
    {
        q_ptr->setPlaceholderText(tr("Search..."));
    }

    QCompleter *completer()
    {
        if (!m_completer)
            createCompleter();
        return m_completer;
    }

    QStringList completions() const
    {
        return (m_completionModel ? m_completionModel->stringList() : QStringList());
    }

    void setCompletions(const QStringList &completions)
    {
        if (!m_completionModel)
            createCompleter();
        m_completionModel->setStringList(completions);
    }

    QAction *iconAction;
    QAction *goAction;

private:
    void createCompleter()
    {
        m_completionModel = new QStringListModel(q_ptr);
        m_completer = new QCompleter(m_completionModel, q_ptr);
        m_completer->setCaseSensitivity(Qt::CaseInsensitive);
        m_completer->setFilterMode(Qt::MatchContains);
        q_ptr->setCompleter(m_completer);
    }

    QStringListModel *m_completionModel;
    QCompleter *m_completer;
};

SearchBar::SearchBar(QWidget *parent)
    : LineEdit(new SearchBarPrivate(this), parent)
{
    WIDGETRY_D(SearchBar);

    addAction(d->iconAction, LeadingPosition);

    addAction(d->goAction, TrailingPosition);
    connect(d->goAction, &QAction::triggered, this, &SearchBar::searchRequested);

    connect(this, &QLineEdit::returnPressed, this, &SearchBar::searchRequested);
    connect(this, &QLineEdit::textEdited, this, &SearchBar::completionsRequested);
}

SearchBar::~SearchBar()
{
}

void SearchBar::addCompletions(const QStringList &completions)
{
    WIDGETRY_D(SearchBar);

    QStringList merged = d->completions() + completions;
    merged.removeDuplicates();
    setCompletions(merged);
}

void SearchBar::clearCompletions()
{
    setCompletions(QStringList());
}

void SearchBar::setCompletions(const QStringList &completions)
{
    WIDGETRY_D(SearchBar);

    const int maxCount = 50;

    if (completions.size() > maxCount)
        d->setCompletions(completions.last(maxCount));
    else
        d->setCompletions(completions);
}

SearchBar::CompletionMode SearchBar::completionMode() const
{
    WIDGETRY_D(SearchBar);

    if (d->completer())
        return d->completer()->completionMode() == QCompleter::PopupCompletion ? PopupCompletion : InlineCompletion;
    else
        return PopupCompletion;
}

void SearchBar::setCompletionMode(CompletionMode mode)
{
    WIDGETRY_D(SearchBar);

    switch (mode) {
    case PopupCompletion:
        d->completer()->setCompletionMode(QCompleter::PopupCompletion);
        d->completer()->setFilterMode(Qt::MatchContains);
        break;

    case InlineCompletion:
        d->completer()->setCompletionMode(QCompleter::InlineCompletion);
        d->completer()->setFilterMode(Qt::MatchStartsWith);
        break;
    }
}

} // namespace Widgetry
