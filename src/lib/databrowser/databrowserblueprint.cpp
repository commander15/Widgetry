#include "databrowserblueprint.h"
#include "databrowserblueprint_p.h"

#include <Widgetry/private/databrowserblueprinttask_p.h>

#include <QtWidgets/qtoolbutton.h>

using namespace Widgetry::DataBrowserBlueprintUtils;

namespace Widgetry {

DataBrowserBlueprint::DataBrowserBlueprint(DataBrowser *browser)
    : d_ptr(new DataBrowserBlueprintPrivate(this, browser))
{
}

DataBrowserBlueprint::~DataBrowserBlueprint()
{
    const bool init = !d_ptr->data->blueprinted;

    while (!d_ptr->tasks.isEmpty()) {
        AbstractDataBrowserBlueprintTask *task = d_ptr->tasks.dequeue();

        if (!d_ptr->commit)
            task->cleanup();
        else if (init)
            task->initialize();
        else
            task->update();

        delete task;
    }

    if (d_ptr->commit) {
        d_ptr->data->blueprinted = true;
        d_ptr->browser->sync();
    }
}

QIcon DataBrowserBlueprint::icon(const QString &fileName)
{
    const QIcon icon(fileName);
    d_ptr->coreTask()->icon = icon;
    return icon;
}

void DataBrowserBlueprint::icon(const QIcon &icon)
{
    d_ptr->coreTask()->icon = icon;
}

void DataBrowserBlueprint::title(const QString &title)
{
    d_ptr->coreTask()->title = title;
}

QAction *DataBrowserBlueprint::action(const QString &text)
{
    Action action;
    action.text = text;
    return d_ptr->coreTask()->setAction(action);
}

QAction *DataBrowserBlueprint::action(const QIcon &icon, const QString &text)
{
    Action action;
    action.icon = icon;
    action.text = text;
    return d_ptr->coreTask()->setAction(action);
}

void DataBrowserBlueprint::action(QAction *action)
{
    d_ptr->coreTask()->setAction(action);
}

QAbstractButton *DataBrowserBlueprint::button(const QString &name, const QIcon &icon)
{
    return button(name, icon, QString());
}

QAbstractButton *DataBrowserBlueprint::button(const QString &name, const QIcon &icon, const QString &text)
{
    Button button;
    button.icon = icon;
    button.tooltip = text;
    return d_ptr->coreTask()->registerButton(name, button);
}

void DataBrowserBlueprint::button(QAbstractButton *button)
{
    d_ptr->coreTask()->registerButton(button->objectName(), button);
}

void DataBrowserBlueprint::filter(AbstractDataEdit *filter)
{
    d_ptr->coreTask()->registerFilter(filter);
}

int DataBrowserBlueprint::tableColumn(const QString &field, QHeaderView::ResizeMode resizeMode)
{
    return tableColumn(field, field, resizeMode);
}

int DataBrowserBlueprint::tableColumn(const QString &label, const QString &field, QHeaderView::ResizeMode resizeMode)
{
    TableColumn column;
    column.label = label;
    column.field = field;
    column.resizeMode = resizeMode;
    column.useResize = true;
    return d_ptr->tableTask()->addColumn(column);
}

void DataBrowserBlueprint::tableColumn(int index, const QString &label)
{
    TableColumn column;
    column.label = label;
    column.useResize = false;
    d_ptr->tableTask()->addColumn(index, column);
}

void DataBrowserBlueprint::tableDelegate(QAbstractItemDelegate *delegate)
{
    d_ptr->tableTask()->setDelegate(delegate, true);
}

void DataBrowserBlueprint::contextMenuAction(QAction *action)
{
    d_ptr->tableTask()->addContextMenuAction(action->objectName(), action);
}

void DataBrowserBlueprint::edit(AbstractDataEditFactory *factory)
{
    d_ptr->coreTask()->setEditFactory(factory);
}

void DataBrowserBlueprint::dataManager(DataGate::AbstractDataManager *manager)
{
    d_ptr->coreTask()->setDataManager(manager);
}

void DataBrowserBlueprint::cancel()
{
    d_ptr->commit = false;
}

DataBrowser *DataBrowserBlueprint::browser() const
{
    return d_ptr->browser;
}


DataBrowserBlueprintPrivate::DataBrowserBlueprintPrivate(DataBrowserBlueprint *q, DataBrowser *browser)
    : q_ptr(q)
    , browser(browser)
    , ui(browser->ui)
    , data(static_cast<DataBrowserPrivate *>(browser->d_ptr.get()))
    , commit(true)
    , m_coreTask(nullptr)
    , m_tableTask(nullptr)
{
}

DataBrowserCoreTask *DataBrowserBlueprintPrivate::coreTask()
{
    if (!m_coreTask) {
        m_coreTask = new DataBrowserCoreTask(q_ptr);
        tasks.prepend(m_coreTask);
    }
    return m_coreTask;
}

DataBrowserTableTask *DataBrowserBlueprintPrivate::tableTask()
{
    if (!m_tableTask) {
        m_tableTask = new DataBrowserTableTask(q_ptr);
        if (m_coreTask)
            tasks.insert(1, m_tableTask);
        else
            tasks.prepend(m_tableTask);
    }
    return m_tableTask;
}

} // namespace Widgetry
