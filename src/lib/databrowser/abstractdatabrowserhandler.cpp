#include "abstractdatabrowserhandler.h"

using namespace DataGate;

namespace Widgetry {

void AbstractRequestWatcher::requestRunning(const DataRequest &query, qint64 progress, qint64 total)
{
    Q_UNUSED(query);
    Q_UNUSED(progress);
    Q_UNUSED(total);
}

bool AbstractInteractionHandler::showMessage(const DataRequest &query, const DataResponse &response)
{
    Q_UNUSED(query);
    Q_UNUSED(response);
    return false;
}

} // namespace Widgetry
