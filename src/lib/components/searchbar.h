#ifndef WIDGETRY_SEARCHBAR_H
#define WIDGETRY_SEARCHBAR_H

#include <Widgetry/global.h>
#include <Widgetry/lineedit.h>

namespace Widgetry {

class WIDGETRY_EXPORT SearchBar : public LineEdit
{
    Q_OBJECT

public:
    enum CompletionMode {
        PopupCompletion,
        InlineCompletion
    };

    explicit SearchBar(QWidget *parent = nullptr);
    virtual ~SearchBar();

    Q_SLOT void addCompletions(const QStringList &completions);
    Q_SLOT void clearCompletions();
    Q_SLOT void setCompletions(const QStringList &completions);

    CompletionMode completionMode() const;
    void setCompletionMode(CompletionMode mode);

    Q_SIGNAL void completionsRequested(const QString &query);

signals:
    void searchRequested();
};

} // namespace Widgetry

#endif // WIDGETRY_SEARCHBAR_H
