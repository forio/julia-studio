qt4_wrap_cpp(TEST_MOC_SOURCES fvversioncomparatortest.h)

add_executable(FervorTests
  fvversioncomparatortest.cpp ${TEST_MOC_SOURCES}
)

target_link_libraries( FervorTests
  ${QT_LIBRARIES}
  ${QT_QTTEST_LIBRARY}
  Fervor
)

add_test(FervorTests ${CMAKE_CURRENT_BINARY_DIR}/FervorTests)