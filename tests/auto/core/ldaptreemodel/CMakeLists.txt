set(TEST_NAME
    ldaptreemodel
)

set(SOURCES
    ldaptreemodel.cpp
)

set(HEADERS
    ldaptreemodel.h
)

qt5_wrap_cpp(MOC_SOURCES ${HEADERS})

set(SOURCES ${SOURCES} ${HEADERS} ${MOC_SOURCES})

add_executable(${TEST_NAME} ${SOURCES})

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
target_include_directories(${TEST_NAME} PRIVATE ${GKDI_INCLUDE_DIR})
target_link_directories(${TEST_NAME} PRIVATE ${GKDI_LIBRARY_DIR})
target_link_libraries(${TEST_NAME} PRIVATE ${GKDI_LIBRARY})
target_link_libraries(${TEST_NAME} PRIVATE Qt5::Core Qt5::Test)
