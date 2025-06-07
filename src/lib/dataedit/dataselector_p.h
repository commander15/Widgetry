#ifndef WIDGETRY_DATASELECTORWIDGET_P_H
#define WIDGETRY_DATASELECTORWIDGET_P_H

#include "dataselector.h"

#include <QtCore/qtimer.h>

#include <Jsoner/tablemodel.h>

#include <DataGate/dataquery.h>
#include <DataGate/dataresponse.h>

namespace Widgetry {

class DataSelectorPrivate
{
public:
    DataSelectorPrivate(DataSelector *q);

    void fetchData(const QString &query, bool append = false);

    DataSelector *q_ptr;
    bool infiniteLoad;
    QTimer requestTimer;

    DataGate::DataQuery query;
    DataGate::DataResponse response;
    Jsoner::TableModel model;
    DataGate::AbstractDataManager *manager;
};

} // namespace Widgetry

#endif // WIDGETRY_DATASELECTORWIDGET_P_H
