#ifndef WIDGETRY_SEARCHBAR_H
#define WIDGETRY_SEARCHBAR_H

#include <Widgetry/global.h>
#include <Widgetry/lineedit.h>

namespace Widgetry {

class WIDGETRY_EXPORT SearchBar : public LineEdit
{
    Q_OBJECT

public:
    explicit SearchBar(QWidget *parent = nullptr);
    virtual ~SearchBar();

signals:
    void completionsRequested(const QString &query);
    void searchRequested();
};

} // namespace Widgetry

#endif // WIDGETRY_SEARCHBAR_H
