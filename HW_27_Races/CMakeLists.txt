cmake_minimum_required(VERSION 3.15)
project(VehicleRacing)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Подпроекты
add_subdirectory(VehicleRacingLib)
add_subdirectory(VehicleRacingApp)