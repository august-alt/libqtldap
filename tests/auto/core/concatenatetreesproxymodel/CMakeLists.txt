find_package(Qt5 REQUIRED COMPONENTS Core Gui Test)

set(TEST_NAME
    concatenatetreesproxymodeltest
)

set(SOURCES
    concatenatetreesproxymodeltest.cpp
)

set(HEADERS
    concatenatetreesproxymodeltest.h
)

qt5_wrap_cpp(MOC_SOURCES ${HEADERS})

set(SOURCES ${SOURCES} ${HEADERS} ${MOC_SOURCES})

add_executable(${TEST_NAME} ${SOURCES})

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
target_include_directories(${TEST_NAME} PRIVATE ${LIB_QT_LDAP_INCLUDE_DIR})
target_link_directories(${TEST_NAME} PRIVATE ${LIB_QT_LDAP_LIBRARY_DIR})
target_link_libraries(${TEST_NAME} PRIVATE ${LIB_QT_LDAP_CORE_LIBRARY})
target_link_libraries(${TEST_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Test)
