#include "lineedit.h"
#include "lineedit_p.h"

#include <QtGui/qevent.h>

namespace Widgetry {

LineEdit::LineEdit(QWidget *parent)
    : LineEdit(new LineEditPrivate(this), parent)
{
}

LineEdit::LineEdit(LineEditPrivate *d, QWidget *parent)
    : QLineEdit(parent)
    , d_ptr(d)
{
    d->initEdit();
    d->retranslateEdit();
}

LineEdit::~LineEdit()
{
}

QRegularExpression LineEdit::regularExpression() const
{
    return d_ptr->regularExpression();
}

void LineEdit::setRegularExpression(const QRegularExpression &regex)
{
    d_ptr->setRegularExpression(regex);
}

QStringList LineEdit::completions() const
{
    return d_ptr->completions();
}

void LineEdit::addCompletions(const QStringList &completions)
{
    d_ptr->setCompletions(completions + d_ptr->completions());
}

void LineEdit::setCompletions(const QStringList &completions)
{
    d_ptr->setCompletions(completions);
}

LineEdit::CompletionMode LineEdit::completionMode() const
{
    if (d_ptr->completer())
        return d_ptr->completer()->completionMode() == QCompleter::PopupCompletion ? PopupCompletion : InlineCompletion;
    else
        return PopupCompletion;
}

void LineEdit::setCompletionMode(CompletionMode mode)
{
    switch (mode) {
    case PopupCompletion:
        d_ptr->completer()->setCompletionMode(QCompleter::PopupCompletion);
        d_ptr->completer()->setFilterMode(Qt::MatchContains);
        break;

    case InlineCompletion:
        d_ptr->completer()->setCompletionMode(QCompleter::InlineCompletion);
        d_ptr->completer()->setFilterMode(Qt::MatchStartsWith);
        break;
    }
}

void LineEdit::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        d_ptr->retranslateEdit();
        event->accept();
    }

    QLineEdit::changeEvent(event);
}

LineEditPrivate::LineEditPrivate(LineEdit *q)
    : q_ptr(q)
    , maxCompletionCount(100)
    , m_validator(nullptr)
    , m_completionModel(nullptr)
    , m_completer(nullptr)
{
}

void LineEditPrivate::initEdit() {}

void LineEditPrivate::retranslateEdit() {}

QRegularExpression LineEditPrivate::regularExpression() const
{
    return (m_validator ? m_validator->regularExpression() : QRegularExpression());
}

void LineEditPrivate::setRegularExpression(const QRegularExpression &regex)
{
    if (m_validator) {
        m_validator->setRegularExpression(regex);
    } else {
        m_validator = new QRegularExpressionValidator(regex, q_ptr);
        q_ptr->setValidator(m_validator);
    }
}

QStringList LineEditPrivate::completions() const
{
    return (m_completionModel ? m_completionModel->stringList() : QStringList());
}

void LineEditPrivate::setCompletions(const QStringList &completions)
{
    if (!m_completionModel) {
        if (completions.isEmpty())
            return;
        else
            createCompleter();
    }

    if (completions.size() <= maxCompletionCount)
        m_completionModel->setStringList(completions);
    else
        m_completionModel->setStringList(completions.mid(0, maxCompletionCount));
}

QCompleter *LineEditPrivate::completer()
{
    if (!m_completer)
        createCompleter();
    return m_completer;
}

void LineEditPrivate::createCompleter()
{
    m_completionModel = new QStringListModel(q_ptr);

    m_completer = new QCompleter(m_completionModel, q_ptr);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setFilterMode(Qt::MatchContains);

    q_ptr->setCompleter(m_completer);
}

} // namespace Widgetry
