cmake_minimum_required(VERSION 3.14)
project(Task3)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(DinamycLib SHARED Leaver.h Leaver.cpp)
add_executable(Task_3 ${CMAKE_CURRENT_SOURCE_DIR}/Task_3.cpp)
target_link_libraries(Task_3 DinamycLib)