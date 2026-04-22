# the minimum version of CMake.
cmake_minimum_required(VERSION 3.5.0)
project(glmark2)

#set(PNG_TESTS OFF CACHE BOOL "" FORCE)
#set(PNG_TOOLS OFF CACHE BOOL "" FORCE)
#set(PNG_HARDWARE_OPTIMIZATIONS OFF CACHE BOOL "" FORCE)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DZ_U4=unsigned\\ long -DZ_SOLO=1")
#add_subdirectory(libpng1.6)
#set(PNG_FOUND TRUE)
#set(PNG_PNG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libpng1.6)
#set(PNG_LIBRARY png)

add_subdirectory(glmark2)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED PACKAGE_FIND_FILE)
    include(${PACKAGE_FIND_FILE})
endif()

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include)

find_library(
    # Sets the name of the path variable.
    hilog-lib
    # Specifies the name of the NDK library that
    # you want CMake to locate.
    hilog_ndk.z
)

find_library(
    # Sets the name of the path variable.
    libace-lib
    # Specifies the name of the NDK library that
    # you want CMake to locate.
    ace_ndk.z
)

find_library(
    # Sets the name of the path variable.
    libnapi-lib
    # Specifies the name of the NDK library that
    # you want CMake to locate.
    ace_napi.z
)

find_library(
    # Sets the name of the path variable.
    libuv-lib
    # Specifies the name of the NDK library that
    # you want CMake to locate.
    uv
)

add_library(entry SHARED napi_init.cpp manager/plugin_manager.cpp)
target_link_libraries(entry PUBLIC ${hilog-lib} ${libace-lib} ${libnapi-lib} ${libuv-lib} librawfile.z.so glmark2-drm)