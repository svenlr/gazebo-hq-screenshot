cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

if (POLICY CMP0100)
    cmake_policy(SET CMP0100 NEW)
endif ()

set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gazebo REQUIRED)

## Compile as C++14
add_compile_options(-std=c++14)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(gazebo-ogre-rendering-utils)

include_directories(SYSTEM
        ${GAZEBO_INCLUDE_DIRS}
        ${Qt5Core_INCLUDE_DIRS}
        gazebo-ogre-rendering-utils/include
        )
add_library(gazebo_hq_screenshot SHARED ${headers_MOC}
        src/HQScreenshot.cpp)
target_link_libraries(gazebo_hq_screenshot ${GAZEBO_LIBRARIES}
        ${Qt5Core_LIBRARIES}
        ${Qt5Widgets_LIBRARIES}
        ${PROTOBUF_LIBRARIES}
        render_to_texture
        png_writer
        )
