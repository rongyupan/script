cmake_minimum_required(VERSION 3.14)
project(cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# OpenCV
set(OpenCV_DIR /opt/homebrew/opt/opencv/lib/cmake)
find_package( OpenCV 4 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Eigen
include_directories( /opt/homebrew/opt/eigen/include/eigen3 )

message(STATUS "Found " ${OpenCV_INCLUDE_DIRS})

add_executable(GaussDisturb GaussDisturb.cpp)

target_link_libraries(GaussDisturb ${OpenCV_LIBS})
