
add_executable(
    librogtest
    rog.test.cpp
)

target_link_libraries(
        librogtest
    PRIVATE
        librog
)

target_compile_options(
        librogtest
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(
        librogtest
    PRIVATE
        -stdlib=libc++
    )
endif()

target_compile_features(
    librogtest
    PRIVATE
        cxx_std_20
)

set_target_properties(
        librogtest
    PROPERTIES
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        PREFIX ""
)