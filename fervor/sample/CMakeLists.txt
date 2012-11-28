qt4_wrap_ui(SAMPLE_UI_HEADERS mainwindow.ui)
qt4_wrap_cpp(SAMPLE_MOC_SOURCES mainwindow.h)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(FervorSample
  main.cpp mainwindow.cpp mainwindow.ui
  ${SAMPLE_UI_HEADERS} ${SAMPLE_MOC_SOURCES}
)

target_link_libraries( FervorSample
  ${QT_LIBRARIES}
  Fervor
)