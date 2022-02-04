cmake_minimum_required(VERSION 3.19)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

project(Chipulator VERSION 1.0 DESCRIPTION "Chip-8 Emulator/Interpreter" LANGUAGES CXX)

set(SRC
	src/cApp.cpp
	src/cApp.h
	src/cMain.cpp
	src/cMain.h
	src/chip8.cpp
	src/chip8.h
	src/GUI/controlPicker.cpp
	src/GUI/controlPicker.h
	src/GUI/counterListCtrl.cpp
	src/GUI/counterListCtrl.h
	src/GUI/memListCtrl.cpp
	src/GUI/memListCtrl.h
	src/GUI/memoryViewer.cpp
	src/GUI/memoryViewer.h
	src/GUI/regListCtrl.cpp
	src/GUI/regListCtrl.h
	src/GUI/stackListCtrl.cpp
	src/GUI/stackListCtrl.h
	src/Sound/beeper.cpp
	src/Sound/beeper.h
	src/Utility/hexPrint.cpp
	src/Utility/hexPrint.h
	
	 )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Chipulator ${SRC})

SET(wxWidgets_ROOT_DIR "C:/Users/Ryan/source/libs/wx315")
find_package(wxWidgets REQUIRED COMPONENTS net core base)

if(wxWidgets_FOUND)
	target_include_directories(Chipulator PRIVATE ${wxWidgets_INCLUDE_DIRS})
	target_link_libraries(Chipulator PRIVATE ${wxWidgets_LIBRARIES})
else()
	message(FATAL_ERROR "wxWidgets not found!")
endif()
 
find_package(SDL2 REQUIRED)

if(SDL2_FOUND)
	target_include_directories(Chipulator PRIVATE ${SDL2_INCLUDE_DIR})
	target_link_libraries(Chipulator PRIVATE ${SDL2_LIBRARIES})
else()
	#Especially on Windows, cmake sometimes has trouble finding the SDL2 paths
	set(SDL2_INCLUDE_DIR "C:/Users/Ryan/source/libs/SDL2-2.0.20/include")
	set(SDL2_LIBRARIES "C:/Users/Ryan/source/libs/SDL2-2.0.20/lib/x64")
	target_include_directories(Chipulator PRIVATE ${SDL2_INCLUDE_DIR})
	target_link_libraries(Chipulator PRIVATE ${SDL2_LIBRARIES}/SDL2.lib ${SDL2_LIBRARIES}/SDL2main.lib )
endif()