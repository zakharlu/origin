cmake_minimum_required(VERSION 3.10)
project(VehicleRacingLib)


add_library(VehicleRacingLib SHARED 
    Vehicle.cpp
    GroundVehicle.cpp
    AirVehicle.cpp
    Camel.cpp
    FastCamel.cpp
    Centaur.cpp
    AllTerrainBoots.cpp
    MagicCarpet.cpp
    Eagle.cpp
    Broom.cpp
    Race.cpp
)


target_include_directories(VehicleRacingLib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)


 target_compile_definitions(VehicleRacingLib PRIVATE VEHICLERACINGLIB_EXPORTS)
 set_target_properties(VehicleRacingLib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
