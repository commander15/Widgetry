#include "tablewidget.h"
#include "tablewidget_p.h"
#include "ui_tablewidget.h"

#include <QtGui/qevent.h>

#include <DataGate/tablemodel.h>
#include <DataGate/dataresponse.h>
#include <qmenu.h>

using namespace DataGate;

namespace Widgetry {

TableWidget::TableWidget(QWidget *parent)
    : Widget(new TableWidgetPrivate(this, QByteArray()))
    , ui(new Ui::TableWidget)
{
    ui->setupUi(this);

    ui->tableView->installEventFilter(this);
    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &TableWidget::doubleClicked);

    connect(ui->pageInput, &QAbstractSpinBox::editingFinished, this, [this] {
        setPage(ui->pageInput->value());
    });

    ui->previousButton->setEnabled(false);
    connect(ui->previousButton, &QAbstractButton::clicked, this, [this] { setPage(page() - 1); });

    ui->nextButton->setEnabled(false);
    connect(ui->nextButton, &QAbstractButton::clicked, this, [this] { setPage(page() + 1); });
}

TableWidget::~TableWidget()
{
    delete ui;
}

Jsoner::Object TableWidget::currentObject() const
{
    WIDGETRY_D(const TableWidget);
    return (d->model ? d->model->object(ui->tableView->currentIndex().row()) : Jsoner::Object());
}

Jsoner::Array TableWidget::selectedObjects() const
{
    WIDGETRY_D(const TableWidget);

    const QModelIndexList indexes = ui->tableView->selectionModel()->selectedRows();
    Jsoner::Array objects;
    std::transform(indexes.begin(), indexes.end(), std::back_inserter(objects), [d](const QModelIndex &index) {
        return d->model->object(index.row());
    });
    return objects;
}

QModelIndexList TableWidget::selectedRows() const
{
    return ui->tableView->selectionModel()->selectedRows();
}

Jsoner::Object TableWidget::objectAt(const QPoint &pos) const
{
    WIDGETRY_D(const TableWidget);
    return (d->model ? d->model->object(ui->tableView->indexAt(pos).row()) : Jsoner::Object());
}

int TableWidget::page() const
{
    WIDGETRY_D(const TableWidget);
    return (d->model ? d->model->page() : 0);
}

void TableWidget::setPage(int page)
{
    WIDGETRY_D(TableWidget);
    if (d->model) {
        d->model->setPage(page);
        d->model->get();
    }
}

int TableWidget::pageCount() const
{
    WIDGETRY_D(const TableWidget);
    return ui->pageInput->maximum();
}

QTableView *TableWidget::view() const
{
    WIDGETRY_D(const TableWidget);
    return ui->tableView;
}

QMenu *TableWidget::contextMenu() const
{
    WIDGETRY_D(const TableWidget);
    return d->contextMenu;
}

void TableWidget::setContextMenu(QMenu *menu)
{
    WIDGETRY_D(TableWidget);
    d->contextMenu = menu;
}

DataGate::TableModel *TableWidget::model() const
{
    WIDGETRY_D(const TableWidget);
    return d->model;
}

void TableWidget::setModel(DataGate::TableModel *model)
{
    WIDGETRY_D(TableWidget);

    if (d->model == model)
        return;

    ui->tableView->setModel(model);

    if (d->model) {
        disconnect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TableWidget::processSelection);
        disconnect(d->model, &TableModel::finished, this, &TableWidget::processResponse);
    }

    d->model = model;

    if (d->model) {
        connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TableWidget::processSelection);
        connect(d->model, &TableModel::finished, this, &TableWidget::processResponse);
    }
}

bool TableWidget::eventFilter(QObject *watched, QEvent *event)
{
    WIDGETRY_D(TableWidget);

    if (watched == ui->tableView && event->type() == QEvent::ContextMenu) {
        emit contextMenuRequested();

        if (d->contextMenu) {
            const QPoint pos = static_cast<QContextMenuEvent *>(event)->globalPos();
            d->contextMenu->popup(pos);
            event->accept();
            return true;
        }
    }

    return Widget::eventFilter(watched, event);
}

void TableWidget::processSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    emit selectionChanged();
}

void TableWidget::processResponse(const DataGate::DataResponse &response)
{
    if (!response.isSuccess())
        return;

    ui->pageInput->setMaximum(response.pageCount());
    ui->pageInput->setValue(response.page());
    ui->pageInput->setSuffix(QStringLiteral(" / %1").arg(response.pageCount()));
    ui->previousButton->setEnabled(response.page() > 1);
    ui->nextButton->setEnabled(response.page() < response.pageCount());
}

Widgetry::TableWidgetPrivate::TableWidgetPrivate(TableWidget *q, const QByteArray &id)
    : WidgetPrivate(q, id)
    , contextMenu(nullptr)
    , model(nullptr)
{
}

void TableWidgetPrivate::init(Ui::TableWidget *ui)
{
    Q_UNUSED(ui);
}

} // namespace Widgetry
