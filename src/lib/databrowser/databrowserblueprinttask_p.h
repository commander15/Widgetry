#ifndef WIDGETRY_DATABROWSERBLUEPRINTTASK_P_H
#define WIDGETRY_DATABROWSERBLUEPRINTTASK_P_H

#include <Widgetry/databrowser.h>
#include <Widgetry/private/databrowser_p.h>

#include "ui_databrowser.h"

#include <QtWidgets/qheaderview.h>

namespace Widgetry {

namespace DataBrowserBlueprintUtils {

struct VisualItem
{
    virtual ~VisualItem() = default;
    void findOrCreate(DataBrowser *browser) { if (!find(browser)) create(browser); }
    virtual bool find(DataBrowser *browser) = 0;
    virtual void create(DataBrowser *browser) = 0;
    virtual bool apply() = 0;
    virtual void cleanup() = 0;

    QString name;
    QIcon icon;
    QString text;
    QString tooltip;
    bool deletable = false;
};

struct Button : VisualItem
{
    bool find(DataBrowser *browser) override
    {
        button = browser->findChild<QAbstractButton *>(name);
        return button;
    }

    void create(DataBrowser *browser) override
    {
        if (button) return;
        button = new QToolButton(browser);
        button->setObjectName(name);
        button->setIcon(icon);
        button->setText(text);
        button->setToolTip(tooltip);
        deletable = true;
    }

    bool apply() override
    {
        if (!button) return false;
        if (!icon.isNull()) button->setIcon(icon);
        if (!text.isEmpty()) button->setText(text);
        if (!tooltip.isEmpty()) button->setToolTip(tooltip);
        return true;
    }

    void cleanup() override
    {
        if (button && deletable) {
            delete button;
            button = nullptr;
        }
    }

    QAbstractButton *button = nullptr;
};

struct Action : VisualItem
{
    bool find(DataBrowser *browser) override
    {
        const QList<QAction *> actions = browser->actions();
        auto it = std::find_if(actions.begin(), actions.end(), [this](QAction *action) {
            return action->objectName() == name;
        });
        action = (it != actions.end() ? *it : nullptr);
        return action;
    }

    void create(DataBrowser *browser) override
    {
        if (action) return;
        action = new QAction(icon, text, browser);
        action->setToolTip(tooltip);
        deletable = true;
    }

    bool apply() override
    {
        if (!action) return false;
        if (!icon.isNull()) action->setIcon(icon);
        if (!text.isEmpty()) action->setText(text);
        if (!tooltip.isEmpty()) action->setToolTip(tooltip);
        return true;
    }

    void cleanup() override
    {
        if (action && deletable) {
            delete action;
            action = nullptr;
        }
    }

    QAction *action = nullptr;
};


struct TableColumn {
    int index = -1;
    QString label;
    QString field;
    QHeaderView::ResizeMode resizeMode = QHeaderView::Stretch;
    bool useResize = false;
};

}

using namespace DataBrowserBlueprintUtils;

class AbstractDataBrowserBlueprintTask
{
public:
    AbstractDataBrowserBlueprintTask(class DataBrowserBlueprint *blueprint);
    virtual ~AbstractDataBrowserBlueprintTask() = default;
    
    virtual void initialize() = 0;
    virtual void update() = 0;
    virtual void cleanup() {}
    
protected:
    DataBrowser *browser;
    Ui::DataBrowser *ui;
    DataBrowserPrivate *data;
};

class DataBrowserCoreTask : public AbstractDataBrowserBlueprintTask
{
public:
    DataBrowserCoreTask(DataBrowserBlueprint *blueprint);

    void initialize() override;
    void update() override;
    void cleanup() override;

    QAction *setAction(const Action &action);
    void setAction(QAction *action);

    QAbstractButton *registerButton(const QString &name, const Button &button);
    void registerButton(const QString &name, QAbstractButton *button);

    void registerFilter(AbstractDataEdit *filter);

    QAction *registerAction(const QString &name, const Action &action);
    void registerAction(const QString &name, QAction *action);

    void setEditFactory(AbstractDataEditFactory *factory);
    void setDataManager(DataGate::AbstractDataManager *manager);

    QIcon icon;
    QString title;

private:
    void prepare();

    Action m_action;
    QList<Button> m_buttons;
    AbstractDataEdit *m_filter;
    QList<Action> m_actions;
    AbstractDataEditFactory *m_factory;
    DataGate::AbstractDataManager *m_manager;
};

class DataBrowserTableTask : public AbstractDataBrowserBlueprintTask
{
public:
    DataBrowserTableTask(DataBrowserBlueprint *blueprint);

    void initialize() override;
    void update() override;
    void cleanup() override;

    void setDelegate(QAbstractItemDelegate *delegate, bool deletable = true);
    int addColumn(const TableColumn &column);
    void addColumn(int index, const TableColumn &column);
    QAction *addContextMenuAction(const QString &name, const Action &action);
    void addContextMenuAction(const QString &name, QAction *action);

private:
    void sortColumns();

    QAbstractItemDelegate *m_delegate;
    bool m_deletableDelegate;
    QList<TableColumn> columns;
    QList<Action> contextMenuActions;
};

class DataBrowserSimpleTask : public AbstractDataBrowserBlueprintTask
{
public:
    enum Type { Init, Update, Both };
    typedef std::function<void(DataBrowser *, Ui::DataBrowser *, DataBrowserPrivate *)> Callback;

    DataBrowserSimpleTask(const Callback &callback, Type type, DataBrowserBlueprint *blueprint);
    DataBrowserSimpleTask(const Callback &callback, Type type, const Callback &cleanup, DataBrowserBlueprint *blueprint);

    void initialize() override { if (m_init) m_init(browser, ui, data); }
    void update() override { if (m_update) m_update(browser, ui, data); }
    void cleanup() override { if (m_cleanup) m_cleanup(browser, ui, data); }

private:
    Callback m_init;
    Callback m_update;
    Callback m_cleanup;
};

} // namespace Widgetry

#endif // WIDGETRY_DATABROWSERBLUEPRINTTASK_P_H
