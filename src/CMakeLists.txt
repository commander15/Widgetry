qt_add_library(Widgetry)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_headers(Widgetry
    PUBLIC
        global.h config.h
        userinterface.h
        dataedit.h
    PRIVATE
        userinterface_p.h
        dataedit_p.h
)

target_sources(Widgetry
    PRIVATE
        userinterface.cpp
        dataedit.cpp
)

target_link_libraries(Widgetry PUBLIC Qt::Widgets GenO::GenO)

generate_target_headers(Widgetry FOLDER Widgetry)
