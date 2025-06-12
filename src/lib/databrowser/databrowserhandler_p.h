#ifndef WIDGETRY_DATABROWSERHANDLER_H
#define WIDGETRY_DATABROWSERHANDLER_H

#include <Widgetry/global.h>
#include <Widgetry/abstractdatabrowserhandler.h>

namespace Widgetry {

class LoadIndicator;

class WIDGETRY_EXPORT RequestInterceptor : public AbstractRequestInterceptor
{
public:
    void intercept(DataGate::DataRequest &request, RequestType type) override;

private:
    QString m_lastQuery;
    QVariantMap m_lastFilters;
};

class WIDGETRY_EXPORT RequestWatcher : public QObject, public AbstractRequestWatcher
{
    Q_OBJECT

public:
    RequestWatcher();

    bool requestStarted(const DataGate::DataRequest &query) override;
    bool requestFinished(const DataGate::DataRequest &query, const DataGate::DataResponse &response) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QPoint computeLoaderPos(const QSize &viewSize) const;

    LoadIndicator *m_indicator;
};

class WIDGETRY_EXPORT InteractionHandler : public AbstractInteractionHandler
{
public:
    bool showMessage(const DataGate::DataRequest &query, const DataGate::DataResponse &response) override;
    QMessageBox::StandardButton askDeletionConfirmation(const Jsoner::Array &objects) override;
};

} // namespace Widgetry

#endif // WIDGETRY_DATABROWSERHANDLER_H
