cmake_minimum_required(VERSION 3.28)

# Enable experimental standard library module support.
if (CMAKE_VERSION VERSION_GREATER_EQUAL "4.0")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457")
elseif (CMAKE_VERSION VERSION_GREATER_EQUAL "3.30")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
endif()

project(flag_enum
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "Make your enum flags."
)

# ----- CMake Options -----

option(FLAG_ENUM_USE_STD_MODULE "Use standard library module" ON)
option(FLAG_ENUM_TEST "Enable test targets" OFF)

# ----- flag_enum -----

add_library(flag_enum)
target_sources(flag_enum PUBLIC
    FILE_SET CXX_MODULES
    FILES flag_enum.cppm
)
target_compile_features(flag_enum PUBLIC cxx_std_23)

add_library(flag_enum::flag_enum ALIAS flag_enum)

if (FLAG_ENUM_USE_STD_MODULE)
    # Enable standard library module feature for flag_enum CMake target.
    cmake_minimum_required(VERSION 3.30)
    set_target_properties(flag_enum PROPERTIES CXX_MODULE_STD 1)
endif()

# ----- Tests -----

if (FLAG_ENUM_TEST)
    enable_testing()
    add_subdirectory(test)
endif()

# ----- Packaging -----

include(GNUInstallDirs)
install(
    TARGETS flag_enum
    EXPORT "${PROJECT_NAME}Targets"
    FILE_SET CXX_MODULES DESTINATION module/${PROJECT_NAME}
)
install(
    EXPORT "${PROJECT_NAME}Targets"
    DESTINATION cmake/${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
    cmake/config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION cmake/${PROJECT_NAME}
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION cmake/${PROJECT_NAME}
)