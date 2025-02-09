include(CMakePackageConfigHelpers)

configure_package_config_file(
    WidgetryConfig.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/WidgetryConfig.cmake
    INSTALL_DESTINATION lib/cmake/Widgetry
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/WidgetryConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
    ARCH_INDEPENDENT
)

add_subdirectory(lib)

install(FILES WidgetryConfig.cmake WidgetryConfigVersion.cmake
    DESTINATION lib/cmake/Widgetry
)

install(EXPORT WidgetryTargets
    NAMESPACE Widgetry::
    DESTINATION lib/cmake/Widgetry
)
