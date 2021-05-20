cmake_minimum_required(VERSION 3.19)

project(chip8emu VERSION 1.0 DESCRIPTION "Chip-8 Emulator" LANGUAGES CXX)

set(SDL2_INCLUDE_DIR "C:/dev/vclibs/SDL2-2.0.14/include")
set(SDL2_LIBRARY "C:/dev/vclibs/SDL2-2.0.14/lib/x64")

FIND_PACKAGE(Boost REQUIRED COMPONENTS log)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
add_executable(chip8emu chip8emu.cpp chip8.cpp chip8.h "logger.h" "logger.cpp" "emuWindow.h" "emuWindow.cpp")

target_link_libraries(chip8emu PRIVATE Boost::log_setup Boost::log ${SDL2_LIBRARY}/SDL2main.lib ${SDL2_LIBRARY}/SDL2.lib )