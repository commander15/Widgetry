#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <Widgetry/databrowser.h>

class UserInterface : public Widgetry::DataBrowser
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = nullptr);
    ~UserInterface();

public slots:
    void selectIt();
    void translate();
};

#endif // USERINTERFACE_H
