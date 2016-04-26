cmake_minimum_required(VERSION 3.0)

# The version number.
set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
message(STATUS "Module PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "D:/Developpement/Bibliotheques/boost_1_60_0")
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/OpenSSL-Win32/include")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "D:/Developpement/Bibliotheques/boost_1_60_0/stage/lib")

#include(include/CMakeLists.txt)
include(${CMAKE_INCLUDE_DIR}"/group.cmake")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# Discover the projects that use CMake in the subdirectories.
# Note that explicit cmake invocation is required every time a new project is
# added or removed.
file(GLOB entries *)
foreach(entry ${entries})
  if(IS_DIRECTORY ${entry} AND EXISTS ${entry}/CMakeLists.txt)
    add_subdirectory(${entry})
  endif()
endforeach(entry)
