cmake_minimum_required(VERSION 3.10)
project(color_generator)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_NAME "color-generator")

include_directories(${PROJECT_SOURCE_DIR}/inc)
file(GLOB_RECURSE sources src/*.cpp)

add_executable(${EXECUTABLE_NAME} ${sources})
find_package(wxWidgets REQUIRED core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(${EXECUTABLE_NAME} ${wxWidgets_LIBRARIES})
