set(TEST_NAME
    ldapentitytest
)

set(SOURCES
    ldapentitytest.cpp
)

set(HEADERS
    ldapentitytest.h
)

qt5_wrap_cpp(MOC_SOURCES ${HEADERS})

set(SOURCES ${SOURCES} ${HEADERS} ${MOC_SOURCES})

add_executable(${TEST_NAME} ${SOURCES})

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
target_include_directories(${TEST_NAME} PRIVATE ${QTLDAP_INCLUDE_DIR})
target_link_directories(${TEST_NAME} PRIVATE ${QTLDAP_LIBRARY_DIR})
target_link_libraries(${TEST_NAME} PRIVATE ${QTLDAP_LIBRARY})
target_link_libraries(${TEST_NAME} PRIVATE Qt5::Core Qt5::Test)
