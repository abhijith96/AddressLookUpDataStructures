cmake_minimum_required(VERSION 3.24)
project(DHCPAllocator)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/dependencies)

find_package(Boost CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

include_directories(../)

add_subdirectory(src)



