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

void UserEdit::render(const Jsoner::Object &object)
{
    ui->nameInput->setText(object.string("name"));
    ui->scoreInput->setValue(object.integer("score"));
}

void UserEdit::extract(Jsoner::Object &object) const
{
    object.insert("name", ui->nameInput->text());
    object.insert("score", ui->scoreInput->value());
}

bool UserEdit::validateInput()
{
    int nameSize = ui->nameInput->text().size();

    if (nameSize == 0) {
        setValidationError(tr("You have a name, right ?"));
        return false;
    }

    if (nameSize < 2) {
        setValidationError(tr("You're name is 1 caracter lengh, c'mon"));
        return false;
    }

    if (ui->scoreInput->value() == 0) {
        setValidationError(tr("Score is 0, really ?"));
        return false;
    }

    return true;
}

void UserEdit::makeWriteable(bool writeable)
{
    ui->nameInput->setReadOnly(!writeable);
    ui->scoreInput->setEnabled(writeable);
}
