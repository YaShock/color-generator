cmake_minimum_required(VERSION 3.5)
project(color_generator)

set (CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_NAME "color-generator")

include_directories(${PROJECT_SOURCE_DIR}/inc)
file(GLOB_RECURSE sources src/*.cpp)

add_executable(${EXECUTABLE_NAME} ${sources})
find_package(wxWidgets REQUIRED core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(${EXECUTABLE_NAME} ${wxWidgets_LIBRARIES})
