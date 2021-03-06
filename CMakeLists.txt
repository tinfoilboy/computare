cmake_minimum_required(VERSION 3.1)

project (sonne LANGUAGES CXX VERSION 2.2.0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set (SONNE_BASE_SOURCE 
    ${CMAKE_SOURCE_DIR}/source/pch.cpp
    ${CMAKE_SOURCE_DIR}/source/file.cpp
    ${CMAKE_SOURCE_DIR}/source/directory_counter.cpp
    ${CMAKE_SOURCE_DIR}/source/config_generator.cpp
    ${CMAKE_SOURCE_DIR}/source/counter.cpp
    ${CMAKE_SOURCE_DIR}/source/config.cpp)

if(MSVC)
list(APPEND SONNE_BASE_SOURCE ${CMAKE_SOURCE_DIR}/source/sonne.rc)
endif()

set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "disable cxxopts examples")
set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "disable cxxopts tests")
set (YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
set (YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml tools")
set (YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")
set (FMT_TEST OFF CACHE BOOL "disable fmt tests")
set (FMT_DOC OFF CACHE BOOL "disable fmt docs")
set (JSON_BuildTests OFF CACHE INTERNAL "")

option (SONNE_BUILD_TESTS "Whether to enable the test suite for sonne" ON)

add_subdirectory (extern/fmt)
add_subdirectory (extern/Catch2)
add_subdirectory (extern/json)
add_subdirectory (extern/cxxopts)

# execute the embed config script at configure time to keep up to date with default config
execute_process(
    COMMAND python
    ${CMAKE_SOURCE_DIR}/embed_config.py
    ${CMAKE_SOURCE_DIR}/default_config.json
    ${CMAKE_SOURCE_DIR}/source/config_generator.cpp)

add_executable (sonne 
    ${SONNE_BASE_SOURCE}
    ${CMAKE_SOURCE_DIR}/source/main.cpp)

target_include_directories (sonne PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries (sonne nlohmann_json::nlohmann_json fmt::fmt cxxopts::cxxopts Threads::Threads)

if (SONNE_BUILD_TESTS)
    add_executable (sonne_test
        ${SONNE_BASE_SOURCE}
        ${CMAKE_SOURCE_DIR}/tests/main.cpp)

    target_include_directories (sonne_test PUBLIC ${CMAKE_SOURCE_DIR}/include)

    target_link_libraries (sonne_test fmt::fmt nlohmann_json::nlohmann_json Catch2::Catch2 Threads::Threads)

    file (COPY ${CMAKE_SOURCE_DIR}/tests/samples DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file (COPY ${CMAKE_SOURCE_DIR}/tests/dir_walk DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file (COPY ${CMAKE_SOURCE_DIR}/tests/ignore_test DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file (COPY ${CMAKE_SOURCE_DIR}/tests/test_config.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
