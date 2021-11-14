# cmake needs this line
cmake_minimum_required(VERSION 3.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
 
# Define project name
project(cpp)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
set(OpenCV_DIR /opt/homebrew/opt/opencv/lib/cmake/opencv4)
find_package(OpenCV REQUIRED)
 
# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "version: ${OpenCV_VERSION}")
message(STATUS "libraries: ${OpenCV_LIBS}")
message(STATUS "include path: ${OpenCV_INCLUDE_DIRS}")
 
# Add OpenCV headers location to your include paths
include_directories(${OpenCV_INCLUDE_DIRS})

# Eigen 
include_directories("/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3")

# Declare the executable target built from your sources
add_executable(${PROJECT_NAME} src/seg2mask.cpp)
# add_executable(${PROJECT_NAME} src/ue42world.cpp)

# Link your application with OpenCV libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
