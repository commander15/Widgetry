target_headers(Widgetry
    PUBLIC
        lineedit.h
        loginedit.h passwordedit.h
        searchbar.h
        tablewidget.h
    PRIVATE
        lineedit_p.h
        tablewidget_p.h
)

target_sources(Widgetry
    PRIVATE
        lineedit.cpp
        loginedit.cpp passwordedit.cpp
        searchbar.cpp
        tablewidget.cpp tablewidget.ui
)
