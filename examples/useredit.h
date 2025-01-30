#ifndef USEREDIT_H
#define USEREDIT_H

#include <Widgetry/dataedit.h>

namespace Ui {
class UserEdit;
}

class UserEdit : public Widgetry::DataEdit
{
    Q_OBJECT

public:
    explicit UserEdit(QWidget *parent = nullptr);
    ~UserEdit();

    void registerFields();

public slots:
    void clear() override;

protected:
    void render(const Jsoner::Object &object, Operation operation) override;
    void extract(Jsoner::Object &object, Operation operation) const override;
    bool validateInput() override;
    void makeWriteable(bool writeable) override;

private:
    Ui::UserEdit *ui;
};

#endif // USEREDIT_H
