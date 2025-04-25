cmake_minimum_required(VERSION 3.21...3.31)

project(mcpelauncherenvironmentchanger)

if(NOT CMAKE_ANDROID_ARCH_ABI MATCHES "^(x86_64|arm64-v8a)$")
    message(FATAL_ERROR "Unsupported ABI")
endif()

set(CMAKE_CXX_STANDARD 23)

set(CPM_SOURCE_CACHE ${PROJECT_SOURCE_DIR}/cpm-cache CACHE PATH "")
include(cmake/CPM.cmake)

add_library(mcpelauncherenvironmentchanger SHARED
        src/main.cpp
        src/menu.hpp
        src/environment.cpp
        src/environment.h
        src/imgui_config.cpp
        src/imgui_config.h
        src/color_conversion.cpp
        src/color_conversion.h
        src/color.h
        src/config.h
        src/dimension.h
        src/util.cpp
        src/util.h
        src/moon_phase.cpp
        src/moon_phase.h
        )

target_compile_options(mcpelauncherenvironmentchanger PRIVATE -Wall -Wextra -Wconversion -Wpedantic -fdeclspec)

CPMAddPackage("gh:BasedInc/libhat#69149a1cee5a0228492826bd71af1a32d73ed87e")
CPMAddPackage("gh:stephenberry/glaze@5.0.1")
target_link_libraries(mcpelauncherenvironmentchanger PRIVATE libhat::libhat glaze::glaze)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(mcpelauncherenvironmentchanger PRIVATE -flto)
    target_link_options(mcpelauncherenvironmentchanger PRIVATE
            -flto -Wl,--version-script,${PROJECT_SOURCE_DIR}/map.txt -Wl,--no-undefined-version)
    set_target_properties(mcpelauncherenvironmentchanger PROPERTIES
            LINK_DEPENDS ${PROJECT_SOURCE_DIR}/map.txt)
endif()

install(TARGETS mcpelauncherenvironmentchanger LIBRARY DESTINATION mods)
