cmake_minimum_required(VERSION 3.20)
project(ublang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC "src/*.h" "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})
