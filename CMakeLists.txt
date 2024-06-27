cmake_minimum_required(VERSION 3.12)

project(libqtldap)

execute_process(
    COMMAND
    bash -c "grep 'Version:' .gear/libqtldap.spec | awk -F ':' '{ print $2 }' | tr -d [:space:]"
    OUTPUT_VARIABLE LIB_QT_LDAP_GEAR_VERSION
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

string(REPLACE "." ";" GEAR_VERSION_LIST ${LIB_QT_LDAP_GEAR_VERSION})
list(GET GEAR_VERSION_LIST 0 LIB_QT_LDAP_VERSION_MAJOR)
list(GET GEAR_VERSION_LIST 1 LIB_QT_LDAP_VERSION_MINOR)
list(GET GEAR_VERSION_LIST 2 LIB_QT_LDAP_VERSION_PATCH)

if(UNIX)
  include(GNUInstallDirs)
else()
    message(FATAL_ERROR "System not UNIX - not implemented yet")
endif()

set(BKRP_IN_BUILD_TREE TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

configure_file(
    "${CMAKE_MODULE_PATH}/QtLDAPConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/QtLDAPConfig.cmake"
    IMMEDIATE @ONLY
)
install(FILES ${CMAKE_BINARY_DIR}/QtLDAPConfig.cmake DESTINATION "${CMAKE_INSTALL_LIBDIR}/qtldap")

configure_file(
    "${CMAKE_MODULE_PATH}/QtLDAPBuildTreeSettings.cmake.in"
    "${CMAKE_BINARY_DIR}/QtLDAPBuildTreeSettings.cmake"
    IMMEDIATE @ONLY
)

export(PACKAGE QtLDAP)

add_subdirectory(examples)

add_subdirectory(src)

option(LIB_QT_LDAP_BUILD_TESTS "Build tests." ON)
option(LIB_QT_LDAP_BUILD_EXAMPLES "Build examples." ON)

enable_testing()
add_subdirectory(tests)
