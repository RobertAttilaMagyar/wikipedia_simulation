cmake_minimum_required(VERSION 3.16)
project(wikipedia_network_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 QUIET)

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

file(GLOB CORE_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM CORE_SRC "${PROJECT_SOURCE_DIR}/src/bindings.cpp")

add_library(wikipedia_core STATIC ${CORE_SRC})
target_include_directories(wikipedia_core PUBLIC ${PROJECT_SOURCE_DIR}/include/wikipedia_module)
target_link_libraries(wikipedia_core PUBLIC spdlog::spdlog fmt::fmt)
set_target_properties(wikipedia_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

set(PYTHON_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/python/wikipedia_module)

pybind11_add_module(_core src/bindings.cpp)
target_link_libraries(_core PRIVATE wikipedia_core Python3::Python)
target_include_directories(_core PRIVATE ${PROJECT_SOURCE_DIR}/include)

set_target_properties(_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PYTHON_PACKAGE_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PYTHON_PACKAGE_DIR}
)

if(WIN32)
    set_target_properties(_core PROPERTIES SUFFIX ".pyd")
endif()

install(TARGETS _core
        LIBRARY DESTINATION python/wikipedia_module
        RUNTIME DESTINATION python/wikipedia_module
)

install(FILES
        ${PROJECT_SOURCE_DIR}/python/wikipedia_module/__init__.py
        ${PROJECT_SOURCE_DIR}/python/wikipedia_module/_core.pyi
        DESTINATION python/wikipedia_module
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/python/
        DESTINATION .
        FILES_MATCHING PATTERN "*.py"
)
