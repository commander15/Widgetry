#ifndef WIDGETRY_DATABROWSERBLUEPRINT_P_H
#define WIDGETRY_DATABROWSERBLUEPRINT_P_H

#include "databrowserblueprint.h"
#include "databrowserblueprinttask_p.h"

#include <QtCore/qqueue.h>

#include <Widgetry/private/databrowser_p.h>
#include <Widgetry/private/databrowserblueprinttask_p.h>

namespace Widgetry {

class AbstractDataBrowserBlueprintTask;

class DataBrowserBlueprintPrivate
{
public:
    DataBrowserBlueprintPrivate(DataBrowserBlueprint *q, DataBrowser *browser);

    DataBrowserCoreTask *coreTask();
    DataBrowserTableTask *tableTask();

    DataBrowserBlueprint *q_ptr;
    DataBrowser *browser;
    Ui::DataBrowser *ui;
    DataBrowserPrivate *data;
    QQueue<AbstractDataBrowserBlueprintTask *> tasks;
    bool commit;

private:
    DataBrowserCoreTask *m_coreTask;
    DataBrowserTableTask *m_tableTask;
};

}

#endif // WIDGETRY_DATABROWSERBLUEPRINT_P_H
