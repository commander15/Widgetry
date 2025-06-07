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
    prepare();

    for (Button &button : m_buttons)
        if (button.apply())
            ui->buttonsContainerLayout->addWidget(button.button);

    QMenu *contextMenu = ui->tableWidget->contextMenu();
    for (Action &action : m_actions)
        if (action.apply())
            contextMenu->addAction(action.action);

    if (m_filter)
        browser->setFilterEdit(m_filter);

    if (m_factory)
        browser->setEditFactory(m_factory);

    if (m_manager)
        browser->setDataManager(m_manager);
}

void DataBrowserCoreTask::update()
{
    prepare();

    for (Button &button : m_buttons)
        button.apply();

    for (Action &action : m_actions)
        action.apply();

    if (m_factory)
        delete m_factory;

    if (m_manager)
        delete m_manager;
}

void DataBrowserCoreTask::cleanup()
{
    m_action.cleanup();

    for (Button &button : m_buttons)
        button.cleanup();

    if (m_filter)
        delete m_filter;

    for (Action &action : m_actions)
        action.cleanup();

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
    std::transform(columns.begin(), columns.end(), std::back_inserter(fields), [](const TableColumn &column) {
        return column.field;
    });
    data->tableModel.setFields(fields);

    QHeaderView *header = tableView->horizontalHeader();
    for (const TableColumn &column : std::as_const(columns))
        header->setSectionResizeMode(column.index, column.resizeMode);

    QMenu *contextMenu = ui->tableWidget->contextMenu();
    for (Action &action : contextMenuActions)
        contextMenu->addAction(action.action);

    update();
}

void DataBrowserTableTask::update()
{
    if (m_delegate && m_deletableDelegate)
        delete m_delegate;

    sortColumns();

    for (const TableColumn &column : std::as_const(columns))
        data->tableModel.setHeaderData(column.index, Qt::Horizontal, column.label);

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

int DataBrowserTableTask::addColumn(const TableColumn &column)
{
    int index = columns.size();
    addColumn(index, column);
    return index;
}

void DataBrowserTableTask::addColumn(int index, const TableColumn &column)
{
    TableColumn newColumn(column);
    newColumn.index = index;
    columns.append(newColumn);
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
    auto comp = [](const TableColumn &c1, const TableColumn &c2) {
        return c1.index < c2.index;
    };

    auto it = std::max_element(columns.begin(), columns.end(), comp);
    int max = (it != columns.end() ? it->index : 0);

    for (int i(0); i < max; ++i) {
        auto it = std::find_if(columns.begin(), columns.end(), [&i](const TableColumn &column) {
            return column.index == i;
        });

        if (it != columns.end())
            continue;

        TableColumn column = {
            .index = i,
            .label = "?"
        };
        columns.append(column);
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
