target_headers(Widgetry
  PUBLIC
      abstractdataedit.h abstractdataeditfactory.h dataeditfactory.h
      dataedit.h
      formdataedit.h
      datawindow.h
      dataselector.h
  PRIVATE
      abstractdataedit_p.h abstractdataeditfactory_p.h
      dataeditdialog_p.h
      dataedit_p.h
      formdataedit_p.h
      datawindow_p.h
      dataselector_p.h
)

target_sources(Widgetry
  PRIVATE
    abstractdataedit.cpp abstractdataeditfactory.cpp
    dataeditdialog.cpp
    dataedit.cpp
    formdataedit.cpp
    datawindow.cpp
    dataselector.cpp dataselector.ui
)
