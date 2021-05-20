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

public:
	//returns pointer to window object
	SDL_Window* getWindow();
	//returns pointer to renderer object
	SDL_Renderer* getRenderer();
	//returns pointer to texture object
	SDL_Texture* getTexture();
	//initializes window
	bool initialize(int width, int height);
	//write pixel data to texture
	void drawScreen(unsigned char* screenData);
};