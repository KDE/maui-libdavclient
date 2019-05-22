include(GNUInstallDirs)

# Check Qt Packages
find_package(
    Qt5

    COMPONENTS
        Core
        Network
        Xml

    REQUIRED
)
###

# Set Custom Variables
set(HEADERS_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/lib${PROJECT_NAME})
###

# Add Source Files
set(
    CPP_SOURCES
  
    WebDAVClient.cpp
    CardDAV.cpp

    dto/WebDAVItem.cpp
    dto/Contact.cpp
    dto/CardDAVResponseItem.cpp

    utils/NetworkHelper.cpp
    utils/WebDAVReply.cpp
    utils/CardDAVReply.cpp
    utils/XMLHelper.cpp
    utils/Environment.cpp
)
###

# Create and Link Executable
add_library(
    ${PROJECT_NAME}
    SHARED

    ${CPP_SOURCES}
)
target_link_libraries(
    ${PROJECT_NAME}

    Qt5::Core
    Qt5::Network
    Qt5::Xml
)
###

# Set Flags for Library
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
#set_target_properties(${PROJECT_NAME} PROPERTIES
#  PUBLIC_HEADER "WebDAVClient.hpp;CardDAV.hpp;dto/WebDAVItem.hpp;dto/CardDAVResponseItem.hpp;dto/Contact.hpp;utils/WebDAVReply.hpp;utils/CardDAVReply.hpp;utils/Environment.hpp"
#)
###

# Set Installables
install(
    TARGETS davclient
    EXPORT libdavclientConfig
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
export(
    EXPORT libdavclientConfig
    FILE libdavclientConfig.cmake
)
install(
    EXPORT libdavclientConfig
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/libdavclient"
)
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/lib/
    DESTINATION ${HEADERS_INSTALL_DIR}
    FILES_MATCHING PATTERN "*.h*"
)
###
