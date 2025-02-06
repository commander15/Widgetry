#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <Widgetry/datawindow.h>

namespace Ui {
class UserWindow;
}

class UserWindow : public Widgetry::DataWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = nullptr);
    ~UserWindow();

protected:
    void render(const Jsoner::Object &object, Operation operation) override;
    void makeWriteable(bool writeable) override;

private:
    Ui::UserWindow *ui;
};

#endif // USERWINDOW_H
