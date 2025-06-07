#ifndef WIDGETRY_TABLEWIDGET_H
#define WIDGETRY_TABLEWIDGET_H

#include <Widgetry/global.h>
#include <Widgetry/widget.h>

#include <QtCore/qitemselectionmodel.h>

namespace Jsoner {
class Object;
class Array;
}

namespace DataGate {
class TableModel;
class DataResponse;
}

class QTableView;
class QMenu;

namespace Widgetry {

namespace Ui {
class TableWidget;
}

class TableWidgetPrivate;
class WIDGETRY_EXPORT TableWidget : public Widget
{
    Q_OBJECT

public:
    explicit TableWidget(QWidget *parent = nullptr);
    virtual ~TableWidget();

    Jsoner::Object currentObject() const;
    Jsoner::Array selectedObjects() const;

    QModelIndexList selectedRows() const;

    Jsoner::Object objectAt(const QPoint &pos) const;

    int page() const;
    Q_SLOT void setPage(int page);
    Q_SIGNAL void pageChanged(int page);

    int pageCount() const;

    QTableView *view() const;

    QMenu *contextMenu() const;
    void setContextMenu(QMenu *menu);

    DataGate::TableModel *model() const;
    void setModel(DataGate::TableModel *model);

    Q_SIGNAL void selectionChanged();
    Q_SIGNAL void doubleClicked(const QModelIndex &index);
    Q_SIGNAL void contextMenuRequested();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Q_SLOT void processSelection(const QItemSelection &selected, const QItemSelection &deselected);
    Q_SLOT void processResponse(const DataGate::DataResponse &response);

    Ui::TableWidget *ui;
};

} // namespace Widgetry

#endif // WIDGETRY_TABLEWIDGET_H
