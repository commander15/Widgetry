target_headers(Widgetry
  PUBLIC
      abstractdataedit.h abstractdataeditfactory.h dataeditfactory.h
      dataedit.h formdataedit.h datawindow.h
      dataeditdialog.h
      dataselector.h
  PRIVATE
      abstractdataedit_p.h abstractdataeditfactory_p.h
      dataedithelper_p.h
      dataedit_p.h formdataedit_p.h datawindow_p.h
      dataedithelperdialog_p.h
      dataselector_p.h
)

target_sources(Widgetry
  PRIVATE
    abstractdataedit.cpp abstractdataeditfactory.cpp
    dataedithelper.cpp
    dataedit.cpp formdataedit.cpp datawindow.cpp
    dataeditdialog.cpp dataedithelperdialog.cpp
    dataselector.cpp dataselector.ui
)
