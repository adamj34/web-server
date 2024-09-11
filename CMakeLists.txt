cmake_minimum_required(VERSION 3.5.0)
project(web-server VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(web-server main.cpp)

target_sources(web-server PRIVATE
    src/http_server.cpp
    src/http_request.cpp
    src/http_response.cpp
    src/endpoint_manager.cpp
	src/utilities/file_handler.cpp
	src/utilities/compression.cpp
	src/headers/header_handler.cpp
)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 
)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.14.1
)

FetchContent_Declare(
	zlib
	GIT_REPOSITORY https://github.com/madler/zlib.git
	GIT_TAG        v1.3.1
)


FetchContent_MakeAvailable(Catch2 spdlog zlib) 

add_executable(tests
	src/http_server.cpp
	src/http_request.cpp
	src/http_response.cpp
	src/endpoint_manager.cpp
	src/utilities/file_handler.cpp
	src/utilities/compression.cpp
	src/headers/header_handler.cpp
	tests/http_request_test.cpp 
	tests/http_response_test.cpp
	tests/http_server_test.cpp
	tests/endpoint_manager_test.cpp
	tests/file_handler_test.cpp
	tests/compression_test.cpp
	tests/header_handler_test.cpp 
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain spdlog::spdlog zlib)
target_link_libraries(web-server PRIVATE spdlog::spdlog zlib)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)

include(CTest)
include(Catch)
enable_testing() 
catch_discover_tests(tests)
