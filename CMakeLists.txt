cmake_minimum_required(VERSION 3.16)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PATCH_VERSION 0.0.${PATCH_VERSION})

project(generator VERSION ${PATCH_VERSION})

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(generator
        main.cpp
        ${SOURCES}
        ${HEADERS}
        )

target_include_directories(generator PRIVATE "${CMAKE_SOURCE_DIR}/src/controller")
target_include_directories(generator PRIVATE "${CMAKE_SOURCE_DIR}/src/thread_pool")
target_include_directories(generator PRIVATE "${CMAKE_SOURCE_DIR}/src/gmi_to_html")
target_include_directories(generator PRIVATE "${CMAKE_SOURCE_DIR}/src")

set_target_properties(generator PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
set_target_properties(generator PROPERTIES LINK_FLAGS "-fsanitize=address -fno-omit-frame-pointer")


###################################################
## add boost
###################################################
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif ()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(
        CONANFILE conanfile.txt
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing)

find_package(Boost 1.72.0
        COMPONENTS
        program_options
        REQUIRED)

target_include_directories( generator PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(generator ${Boost_LIBRARIES})

##


###################################################
## add gtest
###################################################
#configure_file(CMakeLists.txt.in
#        googletest-download/CMakeLists.txt)
#execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
#execute_process(COMMAND ${CMAKE_COMMAND} --build .
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
#
#add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
#        ${CMAKE_BINARY_DIR}/googletest-build)
###


#enable_testing()
#add_subdirectory(test)


set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT mesamsonov@mail.ru)

include(CPack)