cmake_minimum_required(VERSION 3.5.0)
project(web-server VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(web-server main.cpp)

target_sources(web-server PRIVATE
    src/http_server.cpp
    src/http_request.cpp
    src/http_response.cpp
)

include(CTest)
enable_testing()

