cmake_minimum_required(VERSION 3.10)
project(VehicleRacing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем поддиректории
add_subdirectory(VehicleRacingLib)
add_subdirectory(VehicleRacingApp)

add_dependencies(VehicleRacingApp VehicleRacingLib)