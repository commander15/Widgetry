#ifndef WIDGETRY_DATABROWSER_P_H
#define WIDGETRY_DATABROWSER_P_H

#include "databrowser.h"

#include <Widgetry/private/widget_p.h>
#include <Widgetry/abstractdataedit.h>

#include <DataGate/tablemodel.h>

#include <QtCore/qitemselectionmodel.h>

#include <QtWidgets/qmenu.h>

namespace Widgetry {

typedef std::function<void(const Jsoner::Object &)> EditingCallback;

class DataBrowserPrivate : public QObject, public WidgetPrivate
{
    Q_OBJECT

public:
    DataBrowserPrivate(DataBrowser *q, const QByteArray &id);
    virtual ~DataBrowserPrivate();

    void openEdit(const QJsonObject &item, AbstractDataEdit::Operation operation, const EditingCallback &callback);
    bool hasDataFeature(DataGate::AbstractDataController::Feature feature) const;
    Q_SLOT void adaptToSelection(const QItemSelection &selected, const QItemSelection &deselected);

    Ui::DataBrowser *ui;

    QMenu *contextMenu;
    QAction *showAction;
    QAction *editAction;
    QAction *deleteAction;

    DataGate::TableModel tableModel;

    AbstractDataEditFactory *editFactory;

    AbstractDataEdit *filterWidget;

    DataGate::DataQueryProgressCallback progressCallback;
};

}

#endif // WIDGETRY_DATABROWSER_P_H
