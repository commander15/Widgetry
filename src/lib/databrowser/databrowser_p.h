#ifndef WIDGETRY_DATABROWSER_P_H
#define WIDGETRY_DATABROWSER_P_H

#include "databrowser.h"

#include <Widgetry/private/widget_p.h>
#include <Widgetry/abstractdataedit.h>

#include <DataGate/tablemodel.h>

#include <QtCore/qitemselectionmodel.h>

namespace Widgetry {

typedef std::function<void(const Jsoner::Object &)> EditingCallback;

class DataBrowserPrivate : public QObject, public WidgetPrivate
{
    Q_OBJECT

public:
    DataBrowserPrivate(DataBrowser *q, const QByteArray &id);
    virtual ~DataBrowserPrivate() = default;

    void init(Ui::DataBrowser *ui);

    Q_SLOT void fetchSearchSuggestions(const QString &text);

    void openEdit(const QJsonObject &item, AbstractDataEdit::Operation operation, const EditingCallback &callback);
    bool hasDataFeature(DataGate::AbstractDataManager::Feature feature) const;

    Q_SLOT void adaptToSelection();

    Q_SLOT void processModelResponse(const DataGate::DataResponse &response);
    bool processDataResponse(const DataGate::DataQuery &query, const DataGate::DataResponse &response);

    Ui::DataBrowser *ui;

    DataGate::TableModel tableModel;

    AbstractDataEditFactory *editFactory;

    AbstractDataEdit *filterWidget;

    DataGate::DataQueryProgressCallback progressCallback;

    bool blueprinted = false;
};

}

#endif // WIDGETRY_DATABROWSER_P_H
