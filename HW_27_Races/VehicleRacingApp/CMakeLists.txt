project(VehicleRacingApp VERSION 1.0)

# ������� ����������� ����
add_executable(VehicleRacingApp 
    main.cpp
)

# ������� � ����� �����������
target_link_libraries(VehicleRacingApp PRIVATE VehicleRacingLib)

# ��� Windows: �������� DLL ����� � ����������� ������
add_custom_command(TARGET VehicleRacingApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:VehicleRacingLib>
        $<TARGET_FILE_DIR:VehicleRacingApp>
    VERBATIM
)


# ������������� �������� �������
set_target_properties(VehicleRacingApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/VehicleRacingApp
    OUTPUT_NAME "VehicleRacingApp"
)