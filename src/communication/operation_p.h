#ifndef WIDGETRY_OPERATION_P_H
#define WIDGETRY_OPERATION_P_H

#include <QtCore/qvarianthash.h>

namespace Widgetry {

class OperationData : public QSharedData
{
public:
    QString name;
    QVariantHash parameters;
    QVariantHash results;
    QByteArray senderId;
    QByteArray receiverId;
    bool success = false;
};

} // namespace Widgetry

#endif // WIDGETRY_OPERATION_P_H
