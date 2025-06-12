#ifndef WIDGETRY_ABSTRACTDATABROWSERHANDLER_H
#define WIDGETRY_ABSTRACTDATABROWSERHANDLER_H

#include <Widgetry/global.h>

#include <QtCore/qcoreapplication.h>

#include <QtWidgets/qmessagebox.h>

namespace Jsoner {
class Array;
}

namespace DataGate {
class DataRequest;
class DataResponse;
}

namespace Widgetry {

namespace Ui {
class DataBrowser;
}

class DataBrowser;
class DataBrowserPrivate;

class WIDGETRY_EXPORT AbstractDataBrowserHandler
{
    Q_DECLARE_TR_FUNCTIONS(Widgetry::DataBrowser)

public:
    enum HandlerType {
        RequestInterceptor,
        RequestWatcher,
        InteractionHandler
    };

    virtual ~AbstractDataBrowserHandler() = default;

    virtual int handlerType() const = 0;

protected:
    DataBrowser *browser = nullptr;
    Ui::DataBrowser *ui = nullptr;
    DataBrowserPrivate *data = nullptr;

    friend class DataBrowser;
};

class WIDGETRY_EXPORT AbstractRequestInterceptor : public AbstractDataBrowserHandler
{
public:
    enum RequestType {
        IndexRequest,
        ShowRequest,
        StoreRequest,
        UpdateRequest,
        DestroyRequest,

        UnknownRequest
    };

    virtual ~AbstractRequestInterceptor() = default;

    virtual void intercept(DataGate::DataRequest &request, RequestType type) = 0;

    int handlerType() const override final { return RequestInterceptor; }
};

class WIDGETRY_EXPORT AbstractRequestWatcher : public AbstractDataBrowserHandler
{
public:
    virtual ~AbstractRequestWatcher() = default;

    virtual bool requestStarted(const DataGate::DataRequest &query) = 0;
    virtual void requestRunning(const DataGate::DataRequest &query, qint64 progress, qint64 total);
    virtual bool requestFinished(const DataGate::DataRequest &query, const DataGate::DataResponse &response) = 0;

    int handlerType() const override final { return RequestWatcher; }
};

class WIDGETRY_EXPORT AbstractInteractionHandler : public AbstractDataBrowserHandler
{
public:
    virtual ~AbstractInteractionHandler() = default;

    virtual bool showMessage(const DataGate::DataRequest &query, const DataGate::DataResponse &response);
    virtual QMessageBox::StandardButton askDeletionConfirmation(const Jsoner::Array &objects) = 0;

    int handlerType() const override final { return InteractionHandler; }
};

} // namespace Widgetry

#endif // WIDGETRY_ABSTRACTDATABROWSERHANDLER_H
