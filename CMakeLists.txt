cmake_minimum_required(VERSION 2.12)
project(hess_sat)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD 14)

# set(CMAKE_CXX_FLAGS "-mtune=generic -static")

add_executable(hess-sat hess_sat.cc)
