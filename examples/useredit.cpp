#include "useredit.h"
#include "ui_useredit.h"

UserEdit::UserEdit(QWidget *parent)
    : Widgetry::DataEdit(parent)
    , ui(new Ui::UserEdit)
{
    ui->setupUi(this);
}

UserEdit::~UserEdit()
{
    delete ui;
}

void UserEdit::registerFields()
{
    registerField(ui->nameInput);
    registerField(ui->scoreInput);
}

void UserEdit::clear()
{
    ui->nameInput->clear();
    ui->scoreInput->setValue(0);
}

void UserEdit::render(const GenO::Object &object, Operation operation)
{
    ui->nameInput->setText(object.property("name").toString());
    ui->scoreInput->setValue(object.property("score").toInt());
}

void UserEdit::extract(GenO::Object &object, Operation operation) const
{
    object.setProperty("name", ui->nameInput->text());
    object.setProperty("score", ui->scoreInput->value());
}

bool UserEdit::validateInput()
{
    int nameSize = ui->nameInput->text().size();

    if (nameSize == 0) {
        setCompletionError(tr("You have a name, right ?"));
        return false;
    }

    if (nameSize < 2) {
        setCompletionError(tr("You're name is 1 caracter lengh, c'mon"));
        return false;
    }

    if (ui->scoreInput->value() == 0) {
        setCompletionError(tr("Score is 0, really ?"));
        return false;
    }

    return true;
}

void UserEdit::makeWriteable(bool writeable)
{
    ui->nameInput->setReadOnly(!writeable);
    ui->scoreInput->setEnabled(writeable);
}
