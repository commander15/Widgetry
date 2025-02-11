#ifndef WIDGETRY_ABSTRACTDATAEDIT_P_H
#define WIDGETRY_ABSTRACTDATAEDIT_P_H

#include "abstractdataedit.h"

#include <Jsoner/object.h>

#include <QtCore/qstring.h>

namespace Widgetry {

class AbstractDataEditPrivate
{
public:
    explicit AbstractDataEditPrivate(AbstractDataEdit *q);
    virtual ~AbstractDataEditPrivate() = default;

    AbstractDataEdit *q_ptr;

    Jsoner::Object object;
    AbstractDataEdit::Operation operation;
    bool readOnly;

    QString completionErrorString;

    DataEditFinishedCallback finishCallback;

    DataInterface *interface;
};

} // namespace Widgetry

#endif // WIDGETRY_ABSTRACTDATAEDIT_P_H
