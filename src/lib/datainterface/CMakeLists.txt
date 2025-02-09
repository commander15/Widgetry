target_headers(Widgetry
    PUBLIC
        datainterface.h
        datainterfaceblueprint.h
        datainterfaceforge.h
    PRIVATE
        datainterface_p.h
        datainterfaceblueprint_p.h
        datainterfaceforge_p.h
)

target_sources(Widgetry
    PRIVATE
        datainterface.cpp
        datainterfaceblueprint.cpp
        datainterfaceforge.cpp
        datainterface.ui
)

