#include "databrowserblueprinttask_p.h"

#include <Widgetry/abstractdataeditfactory.h>
#include <Widgetry/private/databrowserblueprint_p.h>

#include <QtWidgets/qtableview.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qmenu.h>

namespace Widgetry {

AbstractDataBrowserBlueprintTask::AbstractDataBrowserBlueprintTask(DataBrowserBlueprint *blueprint)
    : browser(blueprint->d_ptr->browser)
    , ui(blueprint->d_ptr->ui)
    , data(blueprint->d_ptr->data)
{
}

DataBrowserCoreTask::DataBrowserCoreTask(DataBrowserBlueprint *blueprint)
    : AbstractDataBrowserBlueprintTask(blueprint)
    , m_filter(nullptr)
    , m_factory(nullptr)
    , m_manager(nullptr)
{
}

void DataBrowserCoreTask::initialize()
{
    update();
}

void DataBrowserCoreTask::update()
{
    prepare();

    for (const LayoutItem &item : std::as_const(topItems)) {
        if (item.widget)
            ui->innerTopLayout->addWidget(item.widget, item.stretch, item.alignment);
        else if (item.layout)
            ui->innerTopLayout->addLayout(item.layout, item.stretch);
        else if (item.spacer)
            ui->innerTopLayout->addSpacerItem(item.spacer);
    }

    for (Button &button : m_buttons)
        if (button.apply() && ui->buttonsContainerLayout->indexOf(button.button) == -1)
            ui->buttonsContainerLayout->addWidget(button.button);

    QMenu *contextMenu = ui->tableWidget->contextMenu();
    const QList<QAction *> contextActions = contextMenu->actions();
    for (Action &action : m_actions)
        if (action.apply() && !contextActions.contains(action.action))
            contextMenu->addAction(action.action);

    if (m_filter)
        browser->setFilterEdit(m_filter);

    if (m_factory)
        browser->setEditFactory(m_factory);

    if (m_manager)
        browser->setDataManager(m_manager);

    complete();
}

void DataBrowserCoreTask::cleanup()
{
    m_action.cleanup();

    for (const LayoutItem &item : std::as_const(topItems)) {
        if (item.widget)
            delete item.widget;
        else if (item.layout)
            delete item.layout;
        else if (item.spacer)
            delete item.spacer;
    }

    for (Button &button : m_buttons)
        button.cleanup();

    if (m_filter)
        delete m_filter;

    for (Action &action : m_actions)
        action.cleanup();

    for (AbstractDataBrowserHandler *handler : std::as_const(handlers))
        delete handler;

    if (m_factory)
        delete m_factory;

    if (m_manager)
        delete m_manager;
}

QAction *DataBrowserCoreTask::setAction(const Action &action)
{
    m_action = action;
    m_action.findOrCreate(browser);
    return m_action.action;
}

void DataBrowserCoreTask::setAction(QAction *action)
{
    m_action.action = action;
    m_action.deletable = false;
}

QAbstractButton *DataBrowserCoreTask::registerButton(const QString &name, const Button &button)
{
    Button newButton(button);
    newButton.name = name;
    newButton.findOrCreate(browser);
    m_buttons.append(newButton);
    return newButton.button;
}

void DataBrowserCoreTask::registerButton(const QString &name, QAbstractButton *button)
{
    Button newButton;
    newButton.name = name;
    newButton.button = button;
    m_buttons.append(newButton);
}

void DataBrowserCoreTask::registerFilter(AbstractDataEdit *filter)
{
    m_filter = filter;
}

QAction *DataBrowserCoreTask::registerAction(const QString &name, const Action &action)
{
    Action newAction(action);
    newAction.name = name;
    newAction.findOrCreate(browser);
    m_actions.append(newAction);
    return newAction.action;
}

void DataBrowserCoreTask::registerAction(const QString &name, QAction *action)
{
    Action newAction;
    newAction.name = name;
    newAction.findOrCreate(browser);
    m_actions.append(newAction);
}

void DataBrowserCoreTask::setEditFactory(AbstractDataEditFactory *factory)
{
    m_factory = factory;
}

void DataBrowserCoreTask::setDataManager(DataGate::AbstractDataManager *manager)
{
    m_manager = manager;
}

void DataBrowserCoreTask::prepare()
{
    if (!icon.isNull())
        browser->setIcon(icon);

    if (!title.isEmpty())
        browser->setTitle(title);

    if (m_action.apply())
        browser->setAction(m_action.action);
}

void DataBrowserCoreTask::complete()
{
    const QStringList parametersNames = parameters.keys();
    for (const QString &name : parametersNames)
        browser->setParameter(name, parameters.value(name));

    for (AbstractDataBrowserHandler *handler : std::as_const(handlers))
        browser->registerHandler(handler);
}

DataBrowserTableTask::DataBrowserTableTask(DataBrowserBlueprint *blueprint)
    : AbstractDataBrowserBlueprintTask(blueprint)
    , m_delegate(nullptr)
    , m_deletableDelegate(true)
{
}

void DataBrowserTableTask::initialize()
{
    QTableView *tableView = ui->tableWidget->view();

    if (m_delegate)
        tableView->setItemDelegate(m_delegate);

    sortColumns();

    QStringList fields;
    std::transform(columns.begin(), columns.end(), std::back_inserter(fields), [](const DataBrowserTableColumnBuilder &column) {
        return column.fieldName();
    });
    data->tableModel.setFields(fields);

    QMenu *contextMenu = ui->tableWidget->contextMenu();
    for (Action &action : contextMenuActions)
        contextMenu->addAction(action.action);

    m_deletableDelegate = false;
    update();
}

void DataBrowserTableTask::update()
{
    if (m_delegate && m_deletableDelegate)
        delete m_delegate;

    sortColumns();

    QHeaderView *header = ui->tableWidget->view()->horizontalHeader();
    for (const DataBrowserTableColumnBuilder &column : std::as_const(columns))
        column.build(header, &data->tableModel);

    for (Action &action : contextMenuActions)
        action.apply();
}

void DataBrowserTableTask::cleanup()
{
    if (m_delegate && m_deletableDelegate)
        delete m_delegate;

    for (Action &action : contextMenuActions)
        action.cleanup();
}

void DataBrowserTableTask::setDelegate(QAbstractItemDelegate *delegate, bool deletable)
{
    this->m_delegate = delegate;
    this->m_deletableDelegate = deletable;
}

DataBrowserTableColumnBuilder DataBrowserTableTask::addColumn(const QString &field)
{
    DataBrowserTableColumnBuilder builder(field);
    columns.append(builder);
    return builder;
}

QAction *DataBrowserTableTask::addContextMenuAction(const QString &name, const Action &action)
{
    Action newAction(action);
    newAction.name = name;
    newAction.findOrCreate(browser);
    contextMenuActions.append(newAction);
    return newAction.action;
}

void DataBrowserTableTask::addContextMenuAction(const QString &name, QAction *action)
{
    Action newAction;
    newAction.name = name;
    newAction.action = action;
    contextMenuActions.append(newAction);
}

void DataBrowserTableTask::sortColumns()
{
    auto comp = [this](const DataBrowserTableColumnBuilder &c1, const DataBrowserTableColumnBuilder &c2) {
        return c1.index(&data->tableModel) < c2.index(&data->tableModel);
    };

    auto it = std::max_element(columns.begin(), columns.end(), comp);
    int max = (it != columns.end() ? it->index(&data->tableModel) : 0);

    for (int i(0); i < max; ++i) {
        auto it = std::find_if(columns.begin(), columns.end(), [this, &i](const DataBrowserTableColumnBuilder &column) {
            return column.index(&data->tableModel) == i;
        });

        if (it != columns.end())
            continue;

        DataBrowserTableColumnBuilder column(QString::number(i));
        columns.append(column.label("?"));
    }

    std::sort(columns.begin(), columns.end(), comp);
}

DataBrowserSimpleTask::DataBrowserSimpleTask(const Callback &callback, Type type, DataBrowserBlueprint *blueprint)
    : DataBrowserSimpleTask(callback, type, Callback(), blueprint)
{
}

DataBrowserSimpleTask::DataBrowserSimpleTask(const Callback &callback, Type type, const Callback &cleanup, DataBrowserBlueprint *blueprint)
    : AbstractDataBrowserBlueprintTask(blueprint)
    , m_cleanup(cleanup)
{
    switch (type) {
    case Init:
        m_init = callback;
        break;

    case Update:
        m_update = callback;
        break;

    case Both:
        m_init = callback;
        m_update = callback;
        break;
    }
}

} // namespace Widgetry
