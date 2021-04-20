cmake_minimum_required(VERSION 3.5)

project(bulk_test)
add_executable(bulk_test main.cpp)
target_include_directories(bulk_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(bulk_test PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

target_link_libraries(bulk_test
  ${CMAKE_THREAD_LIBS_INIT}
  async
  )

if (MSVC)
    target_compile_options(
            bulk_test PRIVATE
            /W4
    )
else ()
    target_compile_options(
            bulk_test PRIVATE
            -Wall -Wextra -pedantic #-Werror
    )
endif()

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)