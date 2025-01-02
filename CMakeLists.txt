cmake_minimum_required(VERSION 2.8)
project(libco)

# This for mac osx only
set(CMAKE_MACOSX_RPATH 0)

# Set lib version
set(LIBCO_VERSION   0.5)

# Set cflags
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-fno-strict-aliasing -O2 -Wall -export-dynamic -Wall -pipe  -D_GNU_SOURCE -D_REENTRANT -fPIC -m32 -Wno-deprecated -DZPort")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS} )
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lws2_32")

# Use c and asm
enable_language(C ASM)

# Add source files
set(SOURCE_FILES
        co_epoll.cpp
        co_hook_sys_call.cpp
        co_routine.cpp
        coctx.cpp
        coctx_swap.S
        wepoll.c)

# Add static and shared library target
add_library(colib_static STATIC ${SOURCE_FILES})
add_library(colib_shared SHARED ${SOURCE_FILES})

target_compile_options(colib_static PUBLIC -DZPort=1)
target_compile_options(colib_shared PUBLIC -DZPort=1)

# Set library output name
set_target_properties(colib_static PROPERTIES OUTPUT_NAME cows)
set_target_properties(colib_shared PROPERTIES OUTPUT_NAME cow)

set_target_properties(colib_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(colib_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# Set shared library version, will generate libcolib.${LIBCO_VERSION}.so and a symbol link named libcolib.so
# For mac osx, the extension name will be .dylib
set_target_properties(colib_shared PROPERTIES VERSION ${LIBCO_VERSION} SOVERSION ${LIBCO_VERSION})
target_link_libraries(colib_shared -lws2_32)



# Macro for add example target
macro(add_example_target EXAMPLE_TARGET)
    add_executable("example_${EXAMPLE_TARGET}" "example_${EXAMPLE_TARGET}.cpp")
    target_link_options("example_${EXAMPLE_TARGET}" PUBLIC -L.)
    target_link_libraries("example_${EXAMPLE_TARGET}" cows pthread -lws2_32)
endmacro(add_example_target)

add_example_target(closure)
add_example_target(cond)
add_example_target(copystack)
add_example_target(echocli)
add_example_target(echosvr)
add_example_target(poll)
add_example_target(setenv)
add_example_target(specific)
add_example_target(thread)

macro(add_example_target2 EXAMPLE_TARGET)
    add_executable("example_${EXAMPLE_TARGET}2" "example_${EXAMPLE_TARGET}.cpp")
    target_link_options("example_${EXAMPLE_TARGET}2" PUBLIC -L.)
    target_link_libraries("example_${EXAMPLE_TARGET}2" cow pthread -lws2_32)
endmacro(add_example_target2)

add_example_target2(closure)
add_example_target2(cond)
add_example_target2(copystack)
add_example_target2(echocli)
add_example_target2(echosvr)
add_example_target2(poll)
add_example_target2(setenv)
add_example_target2(specific)
add_example_target2(thread)
