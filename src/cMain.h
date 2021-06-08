/*
cMain: The main window of the emulator; inherits from wxFrame. Keeps track of UI, UI events, input events, actually drawing the screen buffer from the Chip8, updating the sound machine, and any file IO.
*/

#pragma once

#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>
#include <wx/wx.h>
#include <wx/event.h>
#include <wx/colordlg.h>
#include <wx/numdlg.h>
#include <wx/aboutdlg.h>
#include <SDL.h>
#include "Utility/logger.h"
#include "chip8.h"
#include "Sound/beeper.h"
#include "GUI/controlPicker.h"
#include "GUI/memoryViewer.h"

class cMain : public wxFrame {
public:
	explicit cMain();
	~cMain();

	//Event handling functions
	//Cleanup when this window closes
	void onClose(wxCloseEvent& evt);
	//Stops Chip8, handles state while menu is open
	void onMenuOpen(wxMenuEvent& evt);
	//Starts Chip8, handles state when menu closes
	void onMenuClose(wxMenuEvent& evt);
	//Displays file picker and passes path to helper function to open a ROM
	void onOpenROM(wxCommandEvent& evt);
	//Re-initializes the Chip8
	void onReset(wxCommandEvent& evt);
	//Stops Chip8 cycling
	void onPause(wxCommandEvent& evt);
	//Displays file picker and passes path to helper function to save a state file
	void onSaveState(wxCommandEvent& evt);
	//Displays file picker and passes path to helper function to load a state file
	void onLoadState(wxCommandEvent& evt);
	//Calls "onClose" when exit menu item clicked
	void onExit(wxCommandEvent& evt);
	//Changes color of pixels based on menu item clicked
	void onChangeScreenColors(wxCommandEvent& evt);
	//Changes type of wave in Beeper based on menu item clicked
	void onChangeWaveType(wxCommandEvent& evt);
	//Displays numeric input and passes frequency to Beeper
	void onChangeWaveNote(wxCommandEvent& evt);
	//Silences sound machine
	void onMute(wxCommandEvent& evt);
	//Displays ControlPicker frame and passes pointer to keybinding map
	void onChangeKeys(wxCommandEvent& evt);
	//Displays MemoryViewer frame and sets memoryViewerOpen flag to true
	void onMemoryViewer(wxCommandEvent& evt);
	//Displays About dialog
	void onAbout(wxCommandEvent& evt);
	//Handles input using keybinding map
	void onKeyDown(wxKeyEvent& evt);
	//Contains the render timer, which updates SDL graphics, Beeper, memory viewer based on applicable flags
	void onIdle(wxIdleEvent& evt);
	//Sets memoryViewerOpen flag to false
	void onMemoryViewerClose(wxCommandEvent& evt);
	//Pauses Chip8 (if running) and calls it to cycle once
	void onOneCycle(wxCommandEvent& evt);
	//Displays numeric input, passes new Chip8 cpu rate to Chip8, and restarts cycling so new rate takes effect
	void onEmulationSpeed(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
	//Render timers
	float renderRate = 16.66f;
	std::chrono::time_point<std::chrono::steady_clock> lastTime;
	std::chrono::duration<float, std::milli> accumulator{};

	//Menu bar and menus
	wxMenuBar* m_pMenuBar = nullptr;
	wxMenu* m_pFile = nullptr;
	wxMenu* m_pGraphics = nullptr;
	wxMenu* m_pScreenColors = nullptr;
	wxMenu* m_pSound = nullptr;
	wxMenu* m_pWaveType = nullptr;
	wxMenu* m_pEmulation = nullptr;
	wxMenu* m_pHelp = nullptr;

	//SDL window, handles graphics
	SDL_Window* sdlWindow = NULL;
	//SDL renderer, handles graphics
	SDL_Renderer* sdlRenderer = NULL;
	//Pixel color - white default
	int pColor[3] = { 255, 255, 255 };
	//Background color - black default
	int bColor[3] = { 0, 0, 0 };

	//SDL sound class
	Beeper soundMaker;

	//Chip-8
	Chip8 theChip8;
	//Keeps track of Chip-8 state for temporary pauses
	bool runChip8 = false;
	//Keeps track of if a ROM has been loaded
	bool loadedROM = false;
	//Chip-8 rom size in bytes
	size_t chip8ROMSize;
	//Chip-8 rom array
	unsigned char chip8ROM[3584];

	//Default ROM directory
	std::string defaultROMDir;
	//Default save state directory
	std::string defaultStateDir;
	//Keybinding map (index of Chip-8 input as key, unicode value of key as value)
	std::map<int, int> keybindings;

	//Memory viewer
	MemoryViewer* memViewer = nullptr;
	//Memory viewer open flag
	bool memoryViewerOpen = false;
	//Setter for memory viewer flag
	void setMemoryViewerOpen(bool isOpen);

	//Setting file helpers
	void parseConfigFile();
	//Update config file helper
	void updateConfigFile();
	//Load ROM helper
	void loadROM(std::string filename);
	//Save state helper
	void saveChip8State(std::string filename);
	//Load state helper
	void loadChip8State(std::string filemane);
	//Render helper
	void drawScreen();

	//wxEvent IDs
	enum eventIDs { openROMID, resetID, pauseID, resumeID, saveStateID, loadStateID, exitID, gPID, aPID, wOBID, bOWID, ccPixelID, ccBackID, sineID, squareID, noteID, muteID, changeKeysID, memoryViewerID, emulationSpeedID, oneCycleID, aboutID };
};