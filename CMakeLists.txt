cmake_minimum_required(VERSION 3.5)

project(main)

add_executable(main main.cpp)

set_target_properties(main PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

if (MSVC)
    target_compile_options(
            main PRIVATE
            /W4
    )
else ()
    target_compile_options(
            main PRIVATE
            -Wall -Wextra -pedantic #-Werror
    )
endif()

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
