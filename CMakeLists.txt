cmake_minimum_required(VERSION 3.9)
enable_testing()

project(
    webdavclient
    VERSION 0.1.0
    DESCRIPTION "WebDAV Client Library"
)

# Set Flags
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX /usr)
###

# Miscellaneous
include(FeatureSummary)

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")
###

# Include SubDirectories
add_subdirectory(lib)
add_subdirectory(lib/tests)
###

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
