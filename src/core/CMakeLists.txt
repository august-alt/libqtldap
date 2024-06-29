find_package(Qt5 COMPONENTS Core LinguistTools REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(LIBRARY_NAME
    qtldap-core
)

set(HEADERS
    ldaptreeitem.h
    ldaptreemodel.h
)

set(SOURCES
    ldaptreeitem.cpp
    ldaptreemodel.cpp
)

add_definitions(
    -DQT_LDAP_CORE_LIBRARY
)

add_library(${LIBRARY_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${LIBRARY_NAME} Qt5::Core)

