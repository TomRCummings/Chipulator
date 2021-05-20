#include "emuWindow.h"

SDL_Window* emuWindow::getWindow() {
	return window;
}

SDL_Renderer* emuWindow::getRenderer() {
	return renderer;
}

SDL_Texture* emuWindow::getTexture() {
	return texture;
}

bool emuWindow::initialize(int width, int height) {
	bool initialized = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		ERRORLOG << "SDL could not be initialized! SDL error: " << SDL_GetError();
		initialized = true;
	}
	else {
		INFO << "SDL initialized.";
		window = SDL_CreateWindow("Chip-8 Emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			ERRORLOG << "Window counld not be created! SDL error:" << SDL_GetError();
			initialized = false;
		}
		else {
			INFO << "Window initialized.";
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				ERRORLOG << "Renderer counld not be created! SDL error:" << SDL_GetError();
				initialized = false;
			}
			else {
				INFO << "Renderer initialized.";
				SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0x0);
			}
		}
	}
	return initialized;
}

void emuWindow::drawScreen(unsigned char* screenData) {
	SDL_RenderClear(renderer);
	int windowWidth = 0;
	int windowHeight = 0;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	const int pixelWidth = windowWidth / 64;
	const int pixelHeight = windowHeight / 32;

	SDL_SetRenderDrawColor(renderer, 0x33, 0xFF, 0x00, 0xFF);

	for (int i = 0; i < 2048; i++) {
		if (screenData[i] != 0) {
			SDL_Rect rect;
			rect.y = (i / 64)*pixelHeight;
			rect.x = (i - ((i / 64) * 64))*pixelWidth;
			rect.w = pixelWidth;
			rect.h = pixelHeight;

			//INFO << "Rendering rect at pos (" << rect.x << ", " << rect.y << ") with pixel loc (" << (rect.x/pixelWidth) << ", " << (rect.y/pixelHeight) << ")";
			//INFO << "i equals " << i;
			
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0x0);
}

bool emuWindow::getQuitFlag() {
	return quit;
}

void emuWindow::setQuitFlag(bool setter) {
	quit = setter;
}

void emuWindow::close() {
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}