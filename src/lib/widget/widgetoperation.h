#ifndef WIDGETRY_OPERATION_H
#define WIDGETRY_OPERATION_H

#include <Widgetry/global.h>

#include <QtCore/qshareddata.h>

namespace Widgetry {

class WidgetOperationData;
class WIDGETRY_EXPORT WidgetOperation
{
public:
    WidgetOperation(const QString &name);
    WidgetOperation(const WidgetOperation &other);
    WidgetOperation(WidgetOperation &&other);
    ~WidgetOperation();

    QString name() const;

    QVariant parameter() const;

    QVariantHash parameters() const;
    void setParameters(const QVariantHash &parameters);

    QByteArray senderId() const;
    void setSenderId(const QByteArray &id);

    QByteArray receiverId() const;
    void setReceiverId(const QByteArray &id);

private:
    QExplicitlySharedDataPointer<WidgetOperationData> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_OPERATION_H
