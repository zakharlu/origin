project(VehicleRacingLib)

# Создаем динамическую библиотеку
add_library(VehicleRacingLib SHARED 
    transport_impl.cpp
)

# Настройки экспорта
target_compile_definitions(VehicleRacingLib 
    PRIVATE VEHICLERACINGLIB_EXPORTS
)
set_target_properties(VehicleRacingLib PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE
    OUTPUT_NAME "VehicleRacing"
)


# Публичные заголовки
target_include_directories(VehicleRacingLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

