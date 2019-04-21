cmake_minimum_required(VERSION 3.13)
project(PPFileSync C)

set(CMAKE_C_STANDARD 99)
find_package(OpenSSL REQUIRED)

add_subdirectory(Source)
