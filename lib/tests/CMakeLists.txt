find_package(
  Qt5
  REQUIRED

  COMPONENTS
    Test
    Network
    Xml
)
find_package(libdavclient REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
  ${PROJECT_NAME}_test

  TestLibDAVClient.cpp
)

target_link_libraries(
    ${PROJECT_NAME}_test

    Qt5::Test
    Qt5::Network
    Qt5::Xml

    libdavclient.so
)

add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)
