cmake_minimum_required(VERSION 3.17)

project(splicing)

add_library(${PROJECT_NAME} STATIC
    src/api.cpp
    src/errors.cpp
    src/memory_permissions.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    include
)

if(MINGW OR UNIX)
    target_compile_options(${PROJECT_NAME} PUBLIC
        # enable diagnostic warnings in project
        -Wall
        -Wextra
        -Wpedantic
        -Weffc++
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
    )
endif()

add_subdirectory(tests)

enable_testing()
add_test(${PROJECT_NAME}_test tests/${PROJECT_NAME}_test)
