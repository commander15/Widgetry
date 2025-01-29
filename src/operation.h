#ifndef WIDGETRY_OPERATION_H
#define WIDGETRY_OPERATION_H

#include <Widgetry/global.h>

#include <QtCore/qshareddata.h>

namespace Widgetry {

class OperationData;
class WIDGETRY_EXPORT Operation
{
public:
    Operation(const QString &name);
    Operation(const Operation &other) = default;
    ~Operation();

    QString name() const;

    QVariantHash parameters() const;
    void setParameters(const QVariantHash &parameters);

    QByteArray senderId() const;
    void setSenderId(const QByteArray &id);

    QByteArray receiverId() const;
    void setReceiverId(const QByteArray &id);

    bool isSuccess() const;
    void setSuccess(bool success);

    QVariantHash results() const;
    void setResults(const QVariantHash &results);

private:
    QExplicitlySharedDataPointer<OperationData> d_ptr;
};

} // namespace Widgetry

#endif // WIDGETRY_OPERATION_H
