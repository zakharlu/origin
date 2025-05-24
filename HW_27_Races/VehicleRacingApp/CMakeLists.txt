project(VehicleRacingApp VERSION 1.0)

# Создаем исполняемый файл
add_executable(VehicleRacingApp 
    main.cpp
)

# Линкуем с нашей библиотекой
target_link_libraries(VehicleRacingApp PRIVATE VehicleRacingLib)

# Для Windows: копируем DLL рядом с исполняемым файлом
add_custom_command(TARGET VehicleRacingApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:VehicleRacingLib>
        $<TARGET_FILE_DIR:VehicleRacingApp>
    VERBATIM
)


# Устанавливаем выходной каталог
set_target_properties(VehicleRacingApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/VehicleRacingApp
    OUTPUT_NAME "VehicleRacingApp"
)