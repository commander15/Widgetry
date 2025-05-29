target_headers(Widgetry
    PUBLIC
        widget.h widgetoperation.h
    PRIVATE
        widget_p.h widgetoperation_p.h
)

target_sources(Widgetry
    PRIVATE
        widget.cpp widgetoperation.cpp
)
