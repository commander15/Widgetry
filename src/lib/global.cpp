#include "global.h"

void _widgetry_init_resources()
{
    Q_INIT_RESOURCE(widgetry_resources);
}

void Widgetry::init(QApplication &app)
{
    Q_UNUSED(app);
    _widgetry_init_resources();
}
