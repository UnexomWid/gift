cmake_minimum_required(VERSION 3.15)
project(gift C)

set(CMAKE_C_STANDARD 99)

if(EMSCRIPTEN)
    message("-- Detected emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX .js)

    if(${CMAKE_BUILD_TYPE} MATCHES [Dd][Ee][Bb][Uu][Gg])
        set(CMAKE_C_FLAGS "-g -s ASSERTIONS=1")
    else()
        set(CMAKE_C_FLAGS "-O3 \
        -s EXPORTED_FUNCTIONS=_malloc,_free \
        -s EXPORTED_RUNTIME_METHODS=allocate,ALLOC_NORMAL,UTF8ToString,intArrayFromString")
    endif()
endif()

message("-- C flags: ${CMAKE_C_FLAGS}")

file(GLOB_RECURSE SOURCE_FILES src/*.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})