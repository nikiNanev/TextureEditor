cmake_minimum_required(VERSION 3.20)

project(textureEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
message(STATUS "Using vcpkg toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

set(CMAKE_CXX_COMPILER "clang++")

file(GLOB_RECURSE SOURCE_DIRS LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/src/*")
file(GLOB_RECURSE INCLUDE_DIRS LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/include/*")

set(INCLUDES "")
set(SOURCES "")

foreach(ITEM ${SOURCE_DIRS})
    if(NOT IS_DIRECTORY ${ITEM})
        list(APPEND SOURCES ${ITEM})
    endif()
endforeach()

foreach(ITEM ${INCLUDE_DIRS})
    if(IS_DIRECTORY ${ITEM})
        list(APPEND INCLUDES ${ITEM})
    endif()
endforeach()

list(APPEND INCLUDES "${CMAKE_SOURCE_DIR}/include/")

message(STATUS "Sources found: ${SOURCES}")
message(STATUS "Include directories found: ${INCLUDES}")

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
)

set(IMGUI_SOURCES
    deps/imgui/backends/imgui_impl_sdl3.cpp
    deps/imgui/backends/imgui_impl_sdlrenderer3.cpp
    deps/imgui/backends/imgui_impl_vulkan.cpp

    deps/imgui/imgui.cpp
    deps/imgui/imgui_demo.cpp
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_tables.cpp
    deps/imgui/imgui_widgets.cpp)

# Add Dear ImGui sources (adjust paths as needed)
add_library(imgui
    ${IMGUI_SOURCES}
)

set(IMGUI_INCLUDES
    ./deps/imgui/
    ./deps/imgui/backends/)

message(STATUS "imgui headers found: ${IMGUI_INCLUDES}")
message(STATUS "imgui sources found: ${IMGUI_SOURCES}")

include_directories(./include ./include/modules ./deps ${IMGUI_INCLUDES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3
    SDL3_image
    imgui
    m
)
