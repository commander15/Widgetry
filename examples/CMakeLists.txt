set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_libraries(Widgetry)

qt_add_executable(Simple main.cpp
    useredit.h useredit.cpp useredit.ui
    userinterface.h userinterface.cpp
    datacontroller.h datacontroller.cpp
    userwindow.h userwindow.cpp userwindow.ui
)
