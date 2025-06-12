target_headers(Widgetry
    PUBLIC
        databrowser.h
        abstractdatabrowserhandler.h
        databrowserblueprint.h
        databrowsertablecolumnbuilder.h
    PRIVATE
        databrowser_p.h
        databrowserhandler_p.h
        databrowserblueprint_p.h
        databrowserblueprinttask_p.h
)

target_sources(Widgetry
    PRIVATE
        databrowser.cpp databrowser.ui
        abstractdatabrowserhandler.cpp databrowserhandler.cpp
        databrowserblueprint.cpp
        databrowserblueprinttask.cpp
        databrowsertablecolumnbuilder.cpp
)
