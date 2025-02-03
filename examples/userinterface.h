#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <Widgetry/datainterface.h>

class UserInterface : public Widgetry::DataInterface
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = nullptr);
    ~UserInterface();
};

#endif // USERINTERFACE_H
