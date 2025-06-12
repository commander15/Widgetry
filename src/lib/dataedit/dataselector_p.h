#ifndef WIDGETRY_DATASELECTORWIDGET_P_H
#define WIDGETRY_DATASELECTORWIDGET_P_H

#include "dataselector.h"

#include <QtCore/qtimer.h>

#include <Jsoner/tablemodel.h>

#include <DataGate/datarequest.h>
#include <DataGate/dataresponse.h>

namespace Widgetry {

class DataSelectorPrivate
{
public:
    DataSelectorPrivate(DataSelector *q);

    void fetchData(const QString &query, bool append = false);

    DataSelector *q_ptr;
    class LoadIndicator *indicator;

    bool infiniteLoad;
    bool acceptOnClick;
    QTimer requestTimer;

    DataGate::DataRequest request;
    DataGate::DataResponse response;
    Jsoner::TableModel model;
    DataGate::AbstractDataManager *manager;
};

} // namespace Widgetry

#endif // WIDGETRY_DATASELECTORWIDGET_P_H
