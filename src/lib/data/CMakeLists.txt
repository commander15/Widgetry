target_headers(Widgetry
    PUBLIC
        abstractdatacontroller.h
        dataquery.h dataresponse.h
        datatablemodel.h
    PRIVATE
        datatablemodel_p.h
)

target_sources(Widgetry
    PRIVATE
        abstractdatacontroller.cpp
        dataquery.cpp dataresponse.cpp
        datatablemodel.cpp
)
