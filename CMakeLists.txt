cmake_minimum_required(VERSION 3.16)
project(wikipedia_network_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find dependencies
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 QUIET)

# If pybind11 not found, try to find it via Python
if(NOT pybind11_FOUND)
    execute_process(
        COMMAND "${Python3_EXECUTABLE}" -m pybind11 --cmakedir
        OUTPUT_VARIABLE pybind11_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    list(APPEND CMAKE_PREFIX_PATH "${pybind11_DIR}")
    find_package(pybind11 REQUIRED)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${Python3_INCLUDE_DIRS})

# Gather source files (bindings.cpp + others)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Create the Python module target (shared library for pybind11)
add_library(wikipedia_module MODULE src/bindings.cpp ${SRC_FILES})
set(PYTHON_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/src_python/wikipedia_sim)
set_target_properties(wikipedia_module PROPERTIES
    PREFIX ""
    OUTPUT_NAME "wikipedia_module"
    # LIBRARY_OUTPUT_DIRECTORY ${PYTHON_PACKAGE_DIR}
)

if(WIN32)
    set_target_properties(wikipedia_module PROPERTIES SUFFIX ".pyd")
endif()

target_include_directories(wikipedia_module PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${Python3_INCLUDE_DIRS}
)

target_link_libraries(wikipedia_module PRIVATE
    pybind11::module
    spdlog::spdlog
    fmt::fmt
    Python3::Python
)

# Optional: create a standalone executable (if needed)
# add_executable(${PROJECT_NAME} ${SRC_FILES})
# target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog fmt::fmt Python3::Python)
# target_include_directories(${PROJECT_NAME} PRIVATE ${Python3_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
