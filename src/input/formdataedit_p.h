#ifndef WIDGETRY_FORMDATAEDIT_P_H
#define WIDGETRY_FORMDATAEDIT_P_H

#include "formdataedit.h"

#include <Widgetry/private/dataedit_p.h>

class QFormLayout;

namespace Widgetry {

class FormDataEditPrivate : public DataEditPrivate
{
public:
    FormDataEditPrivate(FormDataEdit *edit);

    void createLayout();

    QFormLayout *layout;
};

} // namespace Widgetry

#endif // WIDGETRY_FORMDATAEDIT_P_H
