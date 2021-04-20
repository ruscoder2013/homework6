cmake_minimum_required(VERSION 3.5)

project(async VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
find_package(Threads)
add_library(async SHARED threadsafe_queue.h async.h async.cpp)
target_include_directories(async PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(async PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

target_link_libraries(async
  ${CMAKE_THREAD_LIBS_INIT}
  )

if (MSVC)
    target_compile_options(
            async PRIVATE
            /W4
    )
else ()
    target_compile_options(
            async PRIVATE
            -Wall -Wextra -pedantic #-Werror
    )
endif()

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

install(TARGETS async DESTINATION /usr/local/lib)
