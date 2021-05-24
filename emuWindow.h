#pragma once

#include <string>
#include <SDL.h>
#include <SDL_syswm.h>
#include <windows.h>
#include <Commdlg.h>
#include "logger.h"

class emuWindow {
	//UI IDs
	const enum uiID {fileMID, };
	//Window to render to
	SDL_Window* window = NULL;
	//Window renderer
	SDL_Renderer* renderer = NULL;
	//Displayed texture
	SDL_Texture* texture = NULL;
	//Quit flag
	bool quit = false;

	//Pixel off color array (r,g,b)
	Uint8 pOffColor[3] = { 0x0,0x0,0x0 };
	//Pixel on color array (r,g,b)
	Uint8 pOnColor[3] = { 0xFF, 0xFF, 0xFF };

	//Returns WinAPI window handle
	HWND getWindowHandle();
	//Set up WinAPI drop-down menus
	void readyMenus(HWND winWindowRef);

public:
	//Returns pointer to window object
	SDL_Window* getWindow();
	//Returns pointer to renderer object
	SDL_Renderer* getRenderer();
	//Returns pointer to texture object
	SDL_Texture* getTexture();
	//Initializes window
	bool initialize(int width, int height);
	//Write pixel data to texture
	void drawScreen(unsigned char* screenData);
	//Opens a file dialog and returns path string
	std::string openFileDialog();
	//Return the quit flag
	bool getQuitFlag();
	//Set the quit flag
	void setQuitFlag(bool setter);
	//Destroys the pointers to SDL components
	void close();
	void changeScreenColors(Uint8 pOffR, Uint8 pOffG, Uint8 pOffB, Uint8 pOnR, Uint8 pOnG, Uint8 pOnB);
};