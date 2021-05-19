cmake_minimum_required(VERSION 3.19)

project(chip8emu VERSION 1.0 DESCRIPTION "Chip-8 Emulator" LANGUAGES CXX)

#SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost REQUIRED COMPONENTS log)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
add_executable(chip8emu chip8emu.cpp chip8.cpp chip8.h "logger.h" "logger.cpp")

target_link_libraries(chip8emu PRIVATE Boost::log_setup Boost::log)