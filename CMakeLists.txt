cmake_minimum_required(VERSION 3.20)

project(textureEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

find_package(dlib REQUIRED)

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
    external/imgui/backends/imgui_impl_sdl3.cpp
    external/imgui/backends/imgui_impl_sdlrenderer3.cpp
    external/imgui/backends/imgui_impl_vulkan.cpp

    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp)

# Add Dear ImGui sources (adjust paths as needed)
add_library(imgui
    ${IMGUI_SOURCES}
)

set(IMGUI_INCLUDES
    ./external/imgui/
    ./external/imgui/backends/)

message(STATUS "imgui headers found: ${IMGUI_INCLUDES}")
message(STATUS "imgui sources found: ${IMGUI_SOURCES}")

target_include_directories(imgui PUBLIC ./include imgui ./external/imgui/backends ./external/imgui ./external)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    dlib::dlib
    SDL3
    SDL3_image
    imgui
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${IMGUI_INCLUDES}
)

