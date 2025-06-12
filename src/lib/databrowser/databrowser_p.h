#ifndef WIDGETRY_DATABROWSER_P_H
#define WIDGETRY_DATABROWSER_P_H

#include "databrowser.h"

#include <Widgetry/private/widget_p.h>
#include <Widgetry/abstractdataedit.h>
#include <Widgetry/abstractdatabrowserhandler.h>

#include <DataGate/tablemodel.h>

#include <QtCore/qitemselectionmodel.h>

namespace Widgetry {

typedef std::function<void(const Jsoner::Object &)> EditingCallback;

class DataBrowserPrivate : public QObject, public WidgetPrivate
{
    Q_OBJECT

public:
    DataBrowserPrivate(DataBrowser *q, const QByteArray &id);
    virtual ~DataBrowserPrivate();

    void init(Ui::DataBrowser *ui);

    Q_SLOT void fetchSearchSuggestions(const QString &text);

    void beginRequest(const DataGate::DataRequest &query);
    DataGate::DataRequestCallback monitorRequest(const DataGate::DataRequest &query);
    bool endRequest(const DataGate::DataRequest &query, const DataGate::DataResponse &response, bool commit = true);

    void openEdit(const QJsonObject &item, AbstractDataEdit::Operation operation, const EditingCallback &callback);
    bool hasDataFeature(DataGate::AbstractDataManager::Feature feature) const;

    Q_SLOT void adaptToSelection();

    Q_SLOT void processModelResponse(const DataGate::DataResponse &response);

    bool eventFilter(QObject *watched, QEvent *event) override;

    Ui::DataBrowser *ui;

    DataGate::TableModel tableModel;

    AbstractDataEditFactory *editFactory;

    AbstractDataEdit *filterWidget;

    QList<AbstractRequestInterceptor *> requestInterceptors;
    QList<AbstractRequestWatcher *> requestWatchers;
    QList<AbstractInteractionHandler *> interactionHandlers;

    bool blueprinted = false;
};

}

#endif // WIDGETRY_DATABROWSER_P_H
