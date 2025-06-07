target_headers(Widgetry
    PUBLIC
        databrowser.h
        databrowserblueprint.h
    PRIVATE
        databrowser_p.h
        databrowserblueprint_p.h
        databrowserblueprinttask_p.h
)

target_sources(Widgetry
    PRIVATE
        databrowser.cpp databrowser.ui
        databrowserblueprint.cpp
        databrowserblueprinttask.cpp
)
