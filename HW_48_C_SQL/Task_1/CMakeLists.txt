cmake_minimum_required(VERSION 3.20)
project(Task_1)

# Пути к PostgreSQL
set(PostgreSQL_ROOT "C:/Program Files/PostgreSQL/17")
include_directories("${PostgreSQL_ROOT}/include")
link_directories("${PostgreSQL_ROOT}/lib")

add_subdirectory(c:/CPP/libpqxx libpqxx-build)

add_executable(Task_1 Task_1.cpp)

target_compile_features(Task_1 PRIVATE cxx_std_17)
target_link_libraries(Task_1 PRIVATE pqxx)