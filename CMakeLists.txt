cmake_minimum_required(VERSION 2.8)
project(samples_toybrick_rga)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        .
        rockchip_rga)

add_library(librockchip_rga
        rockchip_rga/android_rga.cpp
        rockchip_rga/rockchip_rga.cpp)

add_executable(rga_cpy
        main.cpp)

target_link_libraries(rga_cpy librockchip_rga)

