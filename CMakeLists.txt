cmake_minimum_required(VERSION 3.5.1)

# Set all of our output directories.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") #GCC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wsign-compare -Wno-unused-function -Wno-unused-variable -Wno-format-truncation -Wold-style-declaration")
endif()

# Get the current working branch
execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the number of commits on the working branch
execute_process(
        COMMAND git rev-list HEAD --count
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_COUNT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
add_definitions(-DGIT_COMMIT_COUNT="${GIT_COMMIT_COUNT}")
add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")

if(UNIX)
    add_definitions(-DPLATFORM_UNIX=1)
elseif(WIN32)
    add_definitions(-DPLATFORM_WIN32=1)
endif()

add_subdirectory(rott/audiolib)

project(hrotte)

add_executable(hrotte
        rott/byteordr.c
        rott/cin_actr.c
        rott/cin_efct.c
        rott/cin_evnt.c
        rott/cin_glob.c
        rott/cin_main.c
        rott/cin_util.c
        rott/dosutil.c
        rott/dukemusc.c
        rott/engine.c
        rott/isr.c
        rott/modexlib.c
        rott/rt_actor.c
        rott/rt_battl.c
        rott/rt_build.c
        rott/rt_cfg.c
        rott/rt_com.c
        rott/rt_crc.c
        rott/rt_debug.c
        rott/rt_dmand.c
        rott/rt_door.c
        rott/rt_draw.c
        rott/rt_err.c
        rott/rt_floor.c
        rott/rt_game.c
        rott/rt_in.c
        rott/rt_main.c
        rott/rt_map.c
        rott/rt_menu.c
        rott/rt_msg.c
        rott/rt_net.c
        rott/rt_playr.c
        rott/rt_rand.c
        rott/rt_scale.c
        rott/rt_sound.c
        rott/rt_spbal.c
        rott/rt_sqrt.c
        rott/rt_stat.c
        rott/rt_state.c
        rott/rt_str.c
        rott/rt_swift.c
        rott/rt_ted.c
        rott/rt_util.c
        rott/rt_vid.c
        rott/rt_view.c
        rott/scriplib.c
        rott/watcom.c
        rott/winrott.c
        rott/w_wad.c
        rott/z_zone.c
        )

target_link_libraries(hrotte audiolib SDL2 SDL2_mixer)
