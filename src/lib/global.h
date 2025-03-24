#ifndef WIDGETRY_GLOBAL_H
#define WIDGETRY_GLOBAL_H

#include <Widgetry/config.h>

#ifdef WIDGETRY_SHARED
#   ifdef WIDGETRY_BUILD
#       define WIDGETRY_EXPORT Q_DECL_EXPORT
#   else
#       define WIDGETRY_EXPORT Q_DECL_IMPORT
#   endif
#else
#   define WIDGETRY_EXPORT
#endif

#define WIDGETRY_Q(ClassName) ClassName *q = static_cast<ClassName *>(q_ptr)
#define WIDGETRY_D(ClassName) \
    ClassName##Private *d = static_cast<ClassName##Private *>(qGetPtrHelper(d_ptr))

class QApplication;

namespace Widgetry {
WIDGETRY_EXPORT void init(QApplication &app);
}

#endif // WIDGETRY_GLOBAL_H
