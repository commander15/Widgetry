target_headers(Widgetry
    PUBLIC
        abstractwidget.h widgetrequest.h widgetresponse.h
        widget.h dialog.h
    PRIVATE
        abstractwidget_p.h
        widget_p.h dialog_p.h
)

target_sources(Widgetry
    PRIVATE
        abstractwidget.cpp widgetrequest.cpp
        widget.cpp dialog.cpp
)
