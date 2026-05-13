#ifndef WIDGETRY_ABSTRACTDATAEDIT_P_H
#define WIDGETRY_ABSTRACTDATAEDIT_P_H

#include "abstractdataedit.h"

#include <QtCore/qstring.h>

#include <Jsoner/object.h>

namespace Widgetry {

class AbstractDataEditPrivate
{
public:
    explicit AbstractDataEditPrivate(AbstractDataEdit *q, QWidget *widget);
    virtual ~AbstractDataEditPrivate() = default;

    AbstractDataEdit *q_ptr;
    QWidget *widget;

    Jsoner::Object object;
    AbstractDataEdit::Operation operation;
    bool readOnly;

    QString validationErrorString;

    DataEditFinishedCallback finishCallback;

    DataBrowser *browser;
};

} // namespace Widgetry

#endif // WIDGETRY_ABSTRACTDATAEDIT_P_H
