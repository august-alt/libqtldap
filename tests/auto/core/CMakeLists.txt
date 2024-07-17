find_package(QtLDAP COMPONENTS core REQUIRED)
include_directories(${LIB_QT_LDAP_INCLUDE_DIR})

find_package(Qt5 COMPONENTS Core Test REQUIRED)
set(QT_USE_QTTEST TRUE)

add_subdirectory(concatenatetreesproxymodel)
add_subdirectory(ldapentity)
add_subdirectory(ldaptreeitem)
add_subdirectory(ldaptreemodel)
