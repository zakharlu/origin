cmake_minimum_required(VERSION 3.10)
project(VehicleRacingApp)

# Добавляем исполняемый файл
add_executable(VehicleRacingApp main.cpp)

# Подключаем библиотеку
target_link_libraries(VehicleRacingApp PRIVATE VehicleRacingLib)
add_dependencies(VehicleRacingApp VehicleRacingLib)

# Подключаем заголовочные файлы библиотеки
target_include_directories(VehicleRacingApp PRIVATE 
    ${CMAKE_SOURCE_DIR}/VehicleRacingLib
)

# копируем DLL рядом с исполняемым файлом
add_custom_command(TARGET VehicleRacingApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:VehicleRacingLib>
        $<TARGET_FILE_DIR:VehicleRacingApp>)