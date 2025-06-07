#ifndef WIDGETRY_LINEEDIT_P_H
#define WIDGETRY_LINEEDIT_P_H

#include "lineedit.h"

#include <QtGui/qvalidator.h>

namespace Widgetry {

class LineEditPrivate
{
public:
    LineEditPrivate(LineEdit *q);
    virtual ~LineEditPrivate() = default;

    virtual void initEdit() {}
    virtual void retranslateEdit() {}

    LineEdit *q_ptr;

    QRegularExpressionValidator *validator;
};

}

#endif // WIDGETRY_LINEEDIT_P_H
