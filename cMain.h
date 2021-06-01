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
#include "logger.h"
#include "chip8.h"
#include "beeper.h"

class cMain : public wxFrame {
public:
	explicit cMain();
	~cMain();

	void onMenuOpen(wxMenuEvent& evt);
	void onMenuClose(wxMenuEvent& evt);
	void onOpenROM(wxCommandEvent& evt);
	void onReset(wxCommandEvent& evt);
	void onPause(wxCommandEvent& evt);
	void onSaveState(wxCommandEvent& evt);
	void onLoadState(wxCommandEvent& evt);
	void onExit(wxCommandEvent& evt);
	void onChangeScreenColors(wxCommandEvent& evt);
	void onChangeWaveType(wxCommandEvent& evt);
	void onChangeWaveNote(wxCommandEvent& evt);
	void onMute(wxCommandEvent& evt);
	void onAbout(wxCommandEvent& evt);
	void onKeyDown(wxKeyEvent& evt);
	void onIdle(wxIdleEvent& evt);

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
	wxMenu* m_pHelp = nullptr;

	//SDL window, handles graphics
	SDL_Window* sdlWindow = NULL;
	//SDL renderer, handles graphics
	SDL_Renderer* sdlRenderer = NULL;
	//Pixel color - white default
	int pColor[3] = { 255, 255, 255 };
	//Background color - black default
	int bColor[3] = { 0, 0, 0 };

	Beeper soundMaker;

	chip8 theChip8;
	bool runChip8 = false;

	size_t chip8ROMSize;
	unsigned char chip8ROM[3584];

	//Load ROM helper
	void loadROM(std::string filename);
	//Save state helper
	void saveChip8State(std::string filename);
	//Load state helper
	void loadChip8State(std::string filemane);

	void drawScreen();

	enum eventIDs { openROMID, resetID, pauseID, resumeID, saveStateID, loadStateID, exitID, gPID, aPID, wOBID, bOWID, ccPixelID, ccBackID, sineID, squareID, noteID, muteID, aboutID };
};