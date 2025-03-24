enable_testing()

find_package(GTest REQUIRED)

add_executable(WidgetryTest
    main.cpp
)

target_compile_definitions(WidgetryTest PRIVATE TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/testdata")

target_link_libraries(WidgetryTest PRIVATE GTest::gtest Qt::Test Widgetry)
