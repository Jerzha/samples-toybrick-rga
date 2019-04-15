cmake_minimum_required(VERSION 3.13)
project(samples_toybrick_rga)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        .
        rockchip_rga)

add_library(rockchip_rga
        rockchip_rga/android_rga.cpp
        rockchip_rga/rockchip_rga.cpp)

add_executable(rga_cpy
        main.cpp)
