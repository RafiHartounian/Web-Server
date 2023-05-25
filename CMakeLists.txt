cmake_minimum_required(VERSION 3.10.0)
project(crazy-code)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system log_setup log)
message(STATUS "Boost version: ${Boost_VERSION}")

include_directories(include)

add_library(server_class src/server.cc)
add_library(session_class src/session.cc src/request_parser.cc)
add_library(config_parser src/config_parser.cc)
add_library(request_parser src/request_parser.cc src/reply.cc)
add_library(reply_class src/reply.cc)
add_library(request_echo_handler src/request_echo_handler.cc)
add_library(request_static_handler src/request_static_handler.cc)
add_library(request_error_handler src/request_error_handler.cc)
add_library(request_404_handler src/request_404_handler.cc)
add_library(request_api_handler src/request_api_handler.cc)
add_library(request_health_handler src/request_health_handler.cc)
add_library(echo_handler_factory_lib src/handler_factories/echo_handler_factory.cc)
add_library(static_handler_factory_lib src/handler_factories/static_handler_factory.cc)
add_library(handler404factory_lib src/handler_factories/handler404factory.cc)
add_library(api_handler_factory_lib src/handler_factories/api_handler_factory.cc)
add_library(health_handler_factory_lib src/handler_factories/health_handler_factory.cc)
add_library(mime_types_class src/mime_types.cc)

add_executable(server src/server_main.cc)
target_link_libraries(server echo_handler_factory_lib static_handler_factory_lib api_handler_factory_lib health_handler_factory_lib handler404factory_lib mime_types_class request_echo_handler request_health_handler request_static_handler request_error_handler request_404_handler request_api_handler config_parser request_parser server_class session_class reply_class pthread Boost::system Boost::log_setup Boost::log)
target_link_libraries(session_class echo_handler_factory_lib static_handler_factory_lib api_handler_factory_lib health_handler_factory_lib handler404factory_lib request_404_handler mime_types_class request_echo_handler request_health_handler request_static_handler request_error_handler request_404_handler request_api_handler request_parser reply_class pthread Boost::system)
target_link_libraries(request_static_handler mime_types_class Boost::filesystem)
target_link_libraries(request_api_handler Boost::filesystem)

# Testing
add_executable(config_parser_test tests/config_parser_test.cc)
add_executable(reply_test tests/reply_test.cc)
add_executable(request_parser_test tests/request_parser_test.cc)
add_executable(session_test tests/session_test.cc)
add_executable(server_test tests/server_test.cc)
add_executable(request_echo_handler_test tests/request_echo_handler_test)
add_executable(request_error_handler_test tests/request_error_handler_test)
add_executable(request_health_handler_test tests/request_health_handler_test)
add_executable(request_static_handler_test tests/request_static_handler_test)
add_executable(request_404_handler_test tests/request_404_handler_test)
add_executable(request_api_handler_test tests/request_api_handler_test)
add_executable(mime_types_test tests/mime_types_test)

target_link_libraries(config_parser_test config_parser gtest_main Boost::log_setup Boost::log)
target_link_libraries(reply_test reply_class gtest_main Boost::system)
target_link_libraries(request_parser_test request_parser gtest_main)
target_link_libraries(session_test session_class reply_class gtest_main Boost::log_setup Boost::log)
target_link_libraries(server_test server_class session_class reply_class config_parser gtest_main gmock_main Boost::log_setup Boost::log)
target_link_libraries(request_echo_handler_test request_echo_handler reply_class gtest_main)
target_link_libraries(request_error_handler_test request_error_handler reply_class gtest_main)
target_link_libraries(request_health_handler_test request_health_handler reply_class Boost::filesystem gtest_main Boost::log)
target_link_libraries(request_static_handler_test request_static_handler reply_class mime_types_class Boost::filesystem gtest_main)
target_link_libraries(request_404_handler_test request_404_handler reply_class gtest_main)
target_link_libraries(request_api_handler_test request_api_handler reply_class Boost::filesystem gtest_main Boost::log)
target_link_libraries(mime_types_test mime_types_class gtest_main)

gtest_discover_tests(config_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(reply_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(session_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(server_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_echo_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_error_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_health_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_static_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_404_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(mime_types_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_api_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_test(NAME integration_tests COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration_tests.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_test(NAME api_integration_tests COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/api_integration_tests.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(TARGETS mime_types_class request_echo_handler request_static_handler request_error_handler request_404_handler request_api_handler server_class session_class config_parser request_parser reply_class TESTS mime_types_test request_echo_handler_test request_static_handler_test request_error_handler_test request_404_handler_test request_api_handler_test session_test config_parser_test request_parser_test reply_test server_test)