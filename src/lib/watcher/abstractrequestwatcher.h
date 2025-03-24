#ifndef WIDGETRY_ABSTRACTREQUESTWATCHER_H
#define WIDGETRY_ABSTRACTREQUESTWATCHER_H

#include <Widgetry/global.h>

namespace Widgetry {

class WIDGETRY_EXPORT AbstractRequestWatcher
{
public:
    virtual ~AbstractRequestWatcher() = default;

    virtual void begin() = 0;
    virtual void progress(int current, int total) = 0;
    virtual void end() = 0;
};

} // namespace Widgetry

#endif // WIDGETRY_ABSTRACTREQUESTWATCHER_H
