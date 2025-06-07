#ifndef WIDGETRY_OPERATION_P_H
#define WIDGETRY_OPERATION_P_H

#include <QtCore/qvarianthash.h>

namespace Widgetry {

class WidgetOperationData : public QSharedData
{
public:
    QString name;
    QVariantHash parameters;
    QByteArray senderId;
    QByteArray receiverId;
};

} // namespace Widgetry

#endif // WIDGETRY_OPERATION_P_H
