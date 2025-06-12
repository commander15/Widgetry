#include "databrowserhandler_p.h"

#include "ui_databrowser.h"

#include <Widgetry/databrowser.h>
#include <Widgetry/loadindicator.h>
#include <Widgetry/private/databrowser_p.h>

#include <QtGui/qevent.h>

#include <QtWidgets/qtableview.h>
#include <QtWidgets/qmessagebox.h>

#include <Jsoner/array.h>

#include <DataGate/dataresponse.h>

using namespace DataGate;

namespace Widgetry {

void RequestInterceptor::intercept(DataGate::DataRequest &request, RequestType type)
{
    if (type == AbstractRequestInterceptor::IndexRequest) {
        request.setQuery(ui->searchInput->text());

        if (data->filterWidget)
            request.setFilters(data->filterWidget->object().toVariantMap());

        if (m_lastQuery != request.query() || m_lastFilters != request.filters()) {
            request.setPage(1);
            request.setSort(QString(), Qt::SortOrder::AscendingOrder);
        } else {
            request.setPage(ui->tableWidget->page());
        }

        m_lastQuery = request.query();
        m_lastFilters = request.filters();
    }
}

RequestWatcher::RequestWatcher()
    : m_indicator(nullptr)
{
}

bool RequestWatcher::requestStarted(const DataRequest &query)
{
    if (m_indicator) {
        m_indicator->show();
    } else {
        QTableView *view = ui->tableWidget->view();
        view->installEventFilter(this);

        m_indicator = new LoadIndicator(view);
        m_indicator->resize(200, 200);
        m_indicator->move(computeLoaderPos(view->size()));
    }

    return true;
}

/*!
 * Process request completion.
 * \param query
 * \param response
 * \return true if eveything is fine, false otherwise.
 *
 * \note returning false from this will abort operations in the DataBrowser,
 * for example, if the user double clicked on an item and is waiting for load,
 * returning false here will abort data loading.
 */
bool RequestWatcher::requestFinished(const DataRequest &query, const DataResponse &response)
{
    if (m_indicator)
        m_indicator->hide();
    return true;
}

bool RequestWatcher::eventFilter(QObject *watched, QEvent *event)
{
    if (ui->tableWidget->view() && event->type() == QEvent::Resize) {
        const QSize size = static_cast<QResizeEvent *>(event)->size();
        if (size.width() < 200 || size.height() < 200)
            m_indicator->resize(size * 0.8);
        else
            m_indicator->resize(200, 200);
        m_indicator->move(computeLoaderPos(size));
    }

    return QObject::eventFilter(watched, event);
}

QPoint RequestWatcher::computeLoaderPos(const QSize &viewSize) const
{
    QPoint pos;
    pos.setX((viewSize.width() - m_indicator->width()) / 2);
    pos.setY((viewSize.height() - m_indicator->height()) / 2);
    return pos;
}

/*!
 * \param query
 * \param response
 * \return true if the handler successfully show the message, false otherwise
 * \note returning true from this method block other handlers to be called for message rendering.
 */
bool InteractionHandler::showMessage(const DataRequest &query, const DataResponse &response)
{
    QMessageBox box(browser);
    box.setWindowFlag(Qt::CustomizeWindowHint, true);
    box.setWindowFlag(Qt::WindowStaysOnTopHint, true);

    if (response.isSuccess()) {
        box.setWindowTitle(response.title().isEmpty() ? tr("Message") : response.title());
        box.setIcon(QMessageBox::Information);
    } else {
        box.setWindowTitle(response.title().isEmpty() ? tr("Error") : response.title());
        box.setIcon(QMessageBox::Warning);
    }

    box.setText(response.text());
    box.setInformativeText(response.informativeText());
    box.setDetailedText(response.detailedText());
    box.exec();
    return true;
}

QMessageBox::StandardButton InteractionHandler::askDeletionConfirmation(const Jsoner::Array &objects)
{
    const QString msg = tr("Do you really want to delete these %n item(s)", nullptr, objects.size());
    return QMessageBox::question(browser, tr("Deletion"), msg);
}

} // namespace Widgetry
