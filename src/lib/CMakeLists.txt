add_library(Widgetry)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_headers(Widgetry
    PUBLIC
        widgetry.h global.h config.h
        userinterface.h
        interfaceholder.h
    PRIVATE
        userinterface_p.h
)

target_sources(Widgetry
    PRIVATE
        global.cpp
        userinterface.cpp
        interfaceholder.cpp
        resources/widgetry_resources.qrc
)

add_subdirectory(communication)
add_subdirectory(datainterface)
add_subdirectory(input)

if (BUILD_SHARED_LIBS)
    set(TYPE SHARED)
else()
    set(TYPE STATIC)
endif()

target_include_directories(Widgetry PUBLIC $<INSTALL_INTERFACE:include>  $<INSTALL_INTERFACE:include/Widgetry>)

target_compile_definitions(Widgetry
    PUBLIC
        WIDGETRY_LIB WIDGETRY_${TYPE}
    PRIVATE
        WIDGETRY_BUILD
)

target_link_libraries(Widgetry PUBLIC Qt::Widgets DataGate::DataGate Jsoner::Jsoner)

generate_target_headers(Widgetry FOLDER Widgetry)

install(TARGETS Widgetry
    EXPORT WidgetryTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/Widgetry
)
