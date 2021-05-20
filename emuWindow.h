#pragma once

#include <SDL.h>
#include <windows.h>
#include "logger.h"

class emuWindow {
	//Window to render to
	SDL_Window* window = NULL;
	//Window renderer
	SDL_Renderer* renderer = NULL;
	//Displayed texture
	SDL_Texture* texture = NULL;
	//Quit flag
	bool quit = false;

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
	//Return the quit flag
	bool getQuitFlag();
	//Set the quit flag
	void setQuitFlag(bool setter);
	//Destroys the pointers to SDL components
	void close();
};