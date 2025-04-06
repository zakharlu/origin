cmake_minimum_required(VERSION 3.22.0)
project(HW_24_Task_2)
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../HW_17_Libs/Task_3")
add_executable(HW24_CMake_Task_2 
${SOURCE_DIR}/Task_3.cpp
${SOURCE_DIR}/Equil_Triangle.cpp
${SOURCE_DIR}/Figure.cpp
${SOURCE_DIR}/Isosceles_Triangle.cpp
${SOURCE_DIR}/Parallelogram.cpp
${SOURCE_DIR}/Quad.cpp
${SOURCE_DIR}/Rectangle.cpp
${SOURCE_DIR}/Right_Triangle.cpp
${SOURCE_DIR}/Romb.cpp
${SOURCE_DIR}/Square.cpp
${SOURCE_DIR}/Triangle.cpp
${SOURCE_DIR}/Equil_Triangle.h
${SOURCE_DIR}/Figure.h
${SOURCE_DIR}/Isosceles_Triangle.h
${SOURCE_DIR}/Parallelogram.h
${SOURCE_DIR}/Quad.h
${SOURCE_DIR}/Rectangle.h
${SOURCE_DIR}/Right_Triangle.h
${SOURCE_DIR}/Romb.h
${SOURCE_DIR}/Square.h
${SOURCE_DIR}/Triangle.h)