#ifndef WIDGETRY_DEBUG_P_H
#define WIDGETRY_DEBUG_P_H

#include <QtCore/qloggingcategory.h>

#define widgetryDebug()    qCDebug(widgetry).noquote().nospace()
#define widgetryInfo()     qCInfo(widgetry).noquote().nospace()
#define widgetryWarning()  qCWarning(widgetry).noquote().nospace()
#define widgetryCritical() qCCritical(widgetry).noquote().nospace()

Q_DECLARE_LOGGING_CATEGORY(widgetry)

#endif // WIDGETRY_DEBUG_P_H
