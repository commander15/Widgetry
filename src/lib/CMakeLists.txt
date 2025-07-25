qt_add_library(Widgetry)

set_target_properties(Widgetry PROPERTIES
    OUTPUT_NAME Widgetry
    VERSION     ${PROJECT_VERSION}
)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_headers(Widgetry
    PUBLIC
        widgetry.h
        global.h config.h
    PRIVATE
        debug_p.h
)

target_sources(Widgetry
    PRIVATE
        debug.cpp
        global.cpp
        resources/widgetry_resources.qrc
)

add_subdirectory(app)
add_subdirectory(components)
add_subdirectory(authentication)
add_subdirectory(widget)
add_subdirectory(databrowser)
add_subdirectory(dataedit)
#add_subdirectory(communication)
#add_subdirectory(datainterface)

target_include_directories(Widgetry 
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/Widgetry>
)

target_compile_definitions(Widgetry
    PUBLIC
        $<IF:$<STREQUAL:$<TARGET_PROPERTY:TYPE>,STATIC_LIBRARY>,WIDGETRY_STATIC,WIDGETRY_SHARED>
    INTERFACE
        WIDGETRY_LIB
    PRIVATE
        WIDGETRY_BUILD
)

target_link_libraries(Widgetry PUBLIC Qt::Core Qt::Gui Qt::Widgets DataGate::DataGate Jsoner::Jsoner)

install(CODE "file(REMOVE_RECURSE ${CMAKE_INSTALL_PREFIX}/include/Widgetry)")

install(TARGETS Widgetry
    EXPORT WidgetryTargets
    ARCHIVE        DESTINATION lib
    LIBRARY        DESTINATION lib
    RUNTIME        DESTINATION bin
    PUBLIC_HEADER  DESTINATION include/Widgetry
    #PRIVATE_HEADER DESTINATION include/Widgetry/private
)

generate_target_headers(Widgetry FOLDER Widgetry)
