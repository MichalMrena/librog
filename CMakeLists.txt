cmake_minimum_required(
    VERSION
        3.21
)

project(
        Rog
    VERSION
        1.0
    DESCRIPTION
        "Simple library for writing and managing test"
    LANGUAGES
        CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(
    librog
    librog/rog.cpp
    librog/details/console.cpp
    librog/details/console_output.cpp
)

target_include_directories(
        librog
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_options(
        librog
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
)

target_compile_features(
        librog
    PRIVATE
        cxx_std_20
)

set_target_properties(
        librog
    PROPERTIES
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        PREFIX ""
)

install(
    TARGETS
        librog
    EXPORT
        librog-config
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# TODO use FILE_SET https://cmake.org/cmake/help/latest/command/install.html#directory
install(
    FILES
        librog/rog.hpp
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/librog
)

export(
    TARGETS
        librog
    NAMESPACE
        rog::
    FILE
        ${CMAKE_CURRENT_BINARY_DIR}/LibrogConfig.cmake
)

install(
    EXPORT
        librog-config
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/librog
    NAMESPACE
        rog::
)

add_subdirectory(tests)