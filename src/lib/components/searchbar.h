#ifndef WIDGETRY_SEARCHBAR_H
#define WIDGETRY_SEARCHBAR_H

#include <Widgetry/global.h>

#include <QtWidgets/qlineedit.h>

namespace Widgetry {

class WIDGETRY_EXPORT SearchBar : public QLineEdit
{
    Q_OBJECT

public:
    explicit SearchBar(QWidget *parent = nullptr);
    virtual ~SearchBar();

signals:
    void searchRequested();

protected:
    void retranslateUi();

    void changeEvent(QEvent *event) override;

private:
    QAction *m_iconAction;
    QAction *m_goAction;
};

} // namespace Widgetry

#endif // WIDGETRY_SEARCHBAR_H
