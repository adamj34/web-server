cmake_minimum_required(VERSION 3.20)
project(web_server)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

include_directories(include)

add_executable(server
    main.cpp
    src/http_server.cpp
)

