cmake_minimum_required(VERSION 3.16)
project(wikipedia_network_simulator LANGUAGES CXX)

# ----------------------------
# C++ Standard
# ----------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------
# Dependencies
# ----------------------------
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

# ----------------------------
# Include directories
# ----------------------------
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${Python3_INCLUDE_DIRS}
)

# ----------------------------
# Core library (static)
# ----------------------------
file(GLOB CORE_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM CORE_SRC "${PROJECT_SOURCE_DIR}/src/bindings.cpp")

add_library(wikipedia_core STATIC ${CORE_SRC})
target_include_directories(wikipedia_core PUBLIC ${PROJECT_SOURCE_DIR}/include/wikipedia_module)
target_link_libraries(wikipedia_core PUBLIC spdlog::spdlog fmt::fmt)
set_target_properties(wikipedia_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ----------------------------
# Debug executable
# ----------------------------
add_executable(main main.cpp)

target_link_libraries(main PRIVATE
    wikipedia_core
    spdlog::spdlog
    fmt::fmt
)

# ----------------------------
# Python package output folder
# ----------------------------
set(PYTHON_PACKAGE_DIR ${PROJECT_SOURCE_DIR}/python/wikipedia_module)

# ----------------------------
# pybind11 module
# ----------------------------
pybind11_add_module(_core src/bindings.cpp)
target_link_libraries(_core PRIVATE wikipedia_core Python3::Python)
target_include_directories(_core PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Ensure the module is placed directly in the package folder
set_target_properties(_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PYTHON_PACKAGE_DIR}      # Unix .so
    RUNTIME_OUTPUT_DIRECTORY ${PYTHON_PACKAGE_DIR}      # Windows .pyd
)

# Windows .pyd suffix
if(WIN32)
    set_target_properties(_core PROPERTIES SUFFIX ".pyd")
endif()

# ----------------------------
# Optional: post-build copy for safety
# ----------------------------
# This ensures _core.pyd ends up in the package folder, even if VS puts it in Release/
add_custom_command(TARGET _core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:_core>
        ${PYTHON_PACKAGE_DIR}/_core$<TARGET_FILE_SUFFIX:_core>
    COMMENT "Copying _core.pyd to python package folder"
)

# ----------------------------
# Installation rules
# ----------------------------
install(TARGETS _core
        LIBRARY DESTINATION python/wikipedia_module
        RUNTIME DESTINATION python/wikipedia_module
)

install(FILES
        ${PROJECT_SOURCE_DIR}/python/wikipedia_module/__init__.py
        ${PROJECT_SOURCE_DIR}/python/wikipedia_module/_core.pyi
        ${PROJECT_SOURCE_DIR}/python/wikipedia_module/py.typed
        DESTINATION python/wikipedia_module
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/python/
        DESTINATION .
        FILES_MATCHING PATTERN "*.py"
)