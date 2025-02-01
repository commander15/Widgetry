target_headers(Widgetry
  PUBLIC
      interfaceserver.h
      operation.h
  PRIVATE
      interfaceserver_p.h
      operation_p.h
)

target_sources(Widgetry
  PRIVATE
    interfaceserver.cpp
    operation.cpp
)
