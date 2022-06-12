

function(conan_message MESSAGE_OUTPUT)
    if(NOT CONAN_CMAKE_SILENT_OUTPUT)
        message(${ARGV${0}})
    endif()
endfunction()


macro(conan_find_apple_frameworks FRAMEWORKS_FOUND FRAMEWORKS FRAMEWORKS_DIRS)
    if(APPLE)
        foreach(_FRAMEWORK ${FRAMEWORKS})
            # https://cmake.org/pipermail/cmake-developers/2017-August/030199.html
            find_library(CONAN_FRAMEWORK_${_FRAMEWORK}_FOUND NAME ${_FRAMEWORK} PATHS ${FRAMEWORKS_DIRS} CMAKE_FIND_ROOT_PATH_BOTH)
            if(CONAN_FRAMEWORK_${_FRAMEWORK}_FOUND)
                list(APPEND ${FRAMEWORKS_FOUND} ${CONAN_FRAMEWORK_${_FRAMEWORK}_FOUND})
            else()
                message(FATAL_ERROR "Framework library ${_FRAMEWORK} not found in paths: ${FRAMEWORKS_DIRS}")
            endif()
        endforeach()
    endif()
endmacro()


function(conan_package_library_targets libraries package_libdir deps out_libraries out_libraries_target build_type package_name)
    unset(_CONAN_ACTUAL_TARGETS CACHE)
    unset(_CONAN_FOUND_SYSTEM_LIBS CACHE)
    foreach(_LIBRARY_NAME ${libraries})
        find_library(CONAN_FOUND_LIBRARY NAME ${_LIBRARY_NAME} PATHS ${package_libdir}
                     NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
        if(CONAN_FOUND_LIBRARY)
            conan_message(STATUS "Library ${_LIBRARY_NAME} found ${CONAN_FOUND_LIBRARY}")
            list(APPEND _out_libraries ${CONAN_FOUND_LIBRARY})
            if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
                # Create a micro-target for each lib/a found
                string(REGEX REPLACE "[^A-Za-z0-9.+_-]" "_" _LIBRARY_NAME ${_LIBRARY_NAME})
                set(_LIB_NAME CONAN_LIB::${package_name}_${_LIBRARY_NAME}${build_type})
                if(NOT TARGET ${_LIB_NAME})
                    # Create a micro-target for each lib/a found
                    add_library(${_LIB_NAME} UNKNOWN IMPORTED)
                    set_target_properties(${_LIB_NAME} PROPERTIES IMPORTED_LOCATION ${CONAN_FOUND_LIBRARY})
                    set(_CONAN_ACTUAL_TARGETS ${_CONAN_ACTUAL_TARGETS} ${_LIB_NAME})
                else()
                    conan_message(STATUS "Skipping already existing target: ${_LIB_NAME}")
                endif()
                list(APPEND _out_libraries_target ${_LIB_NAME})
            endif()
            conan_message(STATUS "Found: ${CONAN_FOUND_LIBRARY}")
        else()
            conan_message(STATUS "Library ${_LIBRARY_NAME} not found in package, might be system one")
            list(APPEND _out_libraries_target ${_LIBRARY_NAME})
            list(APPEND _out_libraries ${_LIBRARY_NAME})
            set(_CONAN_FOUND_SYSTEM_LIBS "${_CONAN_FOUND_SYSTEM_LIBS};${_LIBRARY_NAME}")
        endif()
        unset(CONAN_FOUND_LIBRARY CACHE)
    endforeach()

    if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
        # Add all dependencies to all targets
        string(REPLACE " " ";" deps_list "${deps}")
        foreach(_CONAN_ACTUAL_TARGET ${_CONAN_ACTUAL_TARGETS})
            set_property(TARGET ${_CONAN_ACTUAL_TARGET} PROPERTY INTERFACE_LINK_LIBRARIES "${_CONAN_FOUND_SYSTEM_LIBS};${deps_list}")
        endforeach()
    endif()

    set(${out_libraries} ${_out_libraries} PARENT_SCOPE)
    set(${out_libraries_target} ${_out_libraries_target} PARENT_SCOPE)
endfunction()


include(FindPackageHandleStandardArgs)

conan_message(STATUS "Conan: Using autogenerated Findlibbacktrace.cmake")
# Global approach
set(libbacktrace_FOUND 1)
set(libbacktrace_VERSION "cci.20210118")

find_package_handle_standard_args(libbacktrace REQUIRED_VARS
                                  libbacktrace_VERSION VERSION_VAR libbacktrace_VERSION)
mark_as_advanced(libbacktrace_FOUND libbacktrace_VERSION)


set(libbacktrace_INCLUDE_DIRS "/home/mesam/.conan/data/libbacktrace/cci.20210118/_/_/package/23b828d52c0630e6b0b96d2945419feb7843c4f8/include")
set(libbacktrace_INCLUDE_DIR "/home/mesam/.conan/data/libbacktrace/cci.20210118/_/_/package/23b828d52c0630e6b0b96d2945419feb7843c4f8/include")
set(libbacktrace_INCLUDES "/home/mesam/.conan/data/libbacktrace/cci.20210118/_/_/package/23b828d52c0630e6b0b96d2945419feb7843c4f8/include")
set(libbacktrace_RES_DIRS )
set(libbacktrace_DEFINITIONS )
set(libbacktrace_LINKER_FLAGS_LIST
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:>"
        "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:>"
)
set(libbacktrace_COMPILE_DEFINITIONS )
set(libbacktrace_COMPILE_OPTIONS_LIST "" "")
set(libbacktrace_COMPILE_OPTIONS_C "")
set(libbacktrace_COMPILE_OPTIONS_CXX "")
set(libbacktrace_LIBRARIES_TARGETS "") # Will be filled later, if CMake 3
set(libbacktrace_LIBRARIES "") # Will be filled later
set(libbacktrace_LIBS "") # Same as libbacktrace_LIBRARIES
set(libbacktrace_SYSTEM_LIBS )
set(libbacktrace_FRAMEWORK_DIRS )
set(libbacktrace_FRAMEWORKS )
set(libbacktrace_FRAMEWORKS_FOUND "") # Will be filled later
set(libbacktrace_BUILD_MODULES_PATHS )

conan_find_apple_frameworks(libbacktrace_FRAMEWORKS_FOUND "${libbacktrace_FRAMEWORKS}" "${libbacktrace_FRAMEWORK_DIRS}")

mark_as_advanced(libbacktrace_INCLUDE_DIRS
                 libbacktrace_INCLUDE_DIR
                 libbacktrace_INCLUDES
                 libbacktrace_DEFINITIONS
                 libbacktrace_LINKER_FLAGS_LIST
                 libbacktrace_COMPILE_DEFINITIONS
                 libbacktrace_COMPILE_OPTIONS_LIST
                 libbacktrace_LIBRARIES
                 libbacktrace_LIBS
                 libbacktrace_LIBRARIES_TARGETS)

# Find the real .lib/.a and add them to libbacktrace_LIBS and libbacktrace_LIBRARY_LIST
set(libbacktrace_LIBRARY_LIST backtrace)
set(libbacktrace_LIB_DIRS "/home/mesam/.conan/data/libbacktrace/cci.20210118/_/_/package/23b828d52c0630e6b0b96d2945419feb7843c4f8/lib")

# Gather all the libraries that should be linked to the targets (do not touch existing variables):
set(_libbacktrace_DEPENDENCIES "${libbacktrace_FRAMEWORKS_FOUND} ${libbacktrace_SYSTEM_LIBS} ")

conan_package_library_targets("${libbacktrace_LIBRARY_LIST}"  # libraries
                              "${libbacktrace_LIB_DIRS}"      # package_libdir
                              "${_libbacktrace_DEPENDENCIES}"  # deps
                              libbacktrace_LIBRARIES            # out_libraries
                              libbacktrace_LIBRARIES_TARGETS    # out_libraries_targets
                              ""                          # build_type
                              "libbacktrace")                                      # package_name

set(libbacktrace_LIBS ${libbacktrace_LIBRARIES})

foreach(_FRAMEWORK ${libbacktrace_FRAMEWORKS_FOUND})
    list(APPEND libbacktrace_LIBRARIES_TARGETS ${_FRAMEWORK})
    list(APPEND libbacktrace_LIBRARIES ${_FRAMEWORK})
endforeach()

foreach(_SYSTEM_LIB ${libbacktrace_SYSTEM_LIBS})
    list(APPEND libbacktrace_LIBRARIES_TARGETS ${_SYSTEM_LIB})
    list(APPEND libbacktrace_LIBRARIES ${_SYSTEM_LIB})
endforeach()

# We need to add our requirements too
set(libbacktrace_LIBRARIES_TARGETS "${libbacktrace_LIBRARIES_TARGETS};")
set(libbacktrace_LIBRARIES "${libbacktrace_LIBRARIES};")

set(CMAKE_MODULE_PATH "/home/mesam/.conan/data/libbacktrace/cci.20210118/_/_/package/23b828d52c0630e6b0b96d2945419feb7843c4f8/" ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH "/home/mesam/.conan/data/libbacktrace/cci.20210118/_/_/package/23b828d52c0630e6b0b96d2945419feb7843c4f8/" ${CMAKE_PREFIX_PATH})

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
    # Target approach
    if(NOT TARGET libbacktrace::libbacktrace)
        add_library(libbacktrace::libbacktrace INTERFACE IMPORTED)
        if(libbacktrace_INCLUDE_DIRS)
            set_target_properties(libbacktrace::libbacktrace PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                  "${libbacktrace_INCLUDE_DIRS}")
        endif()
        set_property(TARGET libbacktrace::libbacktrace PROPERTY INTERFACE_LINK_LIBRARIES
                     "${libbacktrace_LIBRARIES_TARGETS};${libbacktrace_LINKER_FLAGS_LIST}")
        set_property(TARGET libbacktrace::libbacktrace PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     ${libbacktrace_COMPILE_DEFINITIONS})
        set_property(TARGET libbacktrace::libbacktrace PROPERTY INTERFACE_COMPILE_OPTIONS
                     "${libbacktrace_COMPILE_OPTIONS_LIST}")
        
    endif()
endif()

foreach(_BUILD_MODULE_PATH ${libbacktrace_BUILD_MODULES_PATHS})
    include(${_BUILD_MODULE_PATH})
endforeach()
