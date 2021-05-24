#include "emuWindow.h"

//WinAPI button IDs
enum buttonIDs { loadRomID, resetID, exitID, controlsID, screenColorsID, beepSoundID, aboutID };

//WinAPI menu objects
static HMENU winMenuBar = NULL;
static HMENU winFile = NULL;
static HMENU winGraphics = NULL;
static HMENU winSound = NULL;
static HMENU winHelp = NULL;

HWND emuWindow::getWindowHandle() {
	//SDL system dependant window info structure
	SDL_SysWMinfo windowInfo;
	SDL_VERSION(&windowInfo.version);

	//Retrieve the WinAPI window handle
	if (!SDL_GetWindowWMInfo(window, &windowInfo)) {
		return NULL;
		ERRORLOG << "Not able to get window handle from WinAPI.";
	}
	else {
		return windowInfo.info.win.window;
	}
}

void emuWindow::readyMenus(HWND winWindowRef) {
	winMenuBar = CreateMenu();
	winFile = CreateMenu();
	winGraphics = CreateMenu();
	winSound = CreateMenu();
	winHelp = CreateMenu();

	//Build menu bar
	AppendMenu(winMenuBar, MF_POPUP, (UINT_PTR)winFile, "File");
	AppendMenu(winMenuBar, MF_POPUP, (UINT_PTR)winGraphics, "Graphics");
	AppendMenu(winMenuBar, MF_POPUP, (UINT_PTR)winSound, "Sound");
	AppendMenu(winMenuBar, MF_POPUP, (UINT_PTR)winHelp, "Help");

	//Build "File" menu
	AppendMenu(winFile, MF_STRING, loadRomID, "Load ROM");
	AppendMenu(winFile, MF_STRING, resetID, "Reset");
	AppendMenu(winFile, MF_STRING, exitID, "Exit");

	//Build "Graphics" menu
	AppendMenu(winGraphics, MF_STRING, screenColorsID, "Screen Colors");

	//Build "Sound" menu
	AppendMenu(winSound, MF_STRING, beepSoundID, "Beep Sound");

	//Build "About" menu
	AppendMenu(winHelp, MF_STRING, aboutID, "About");

	//Attach built menu bar to window
	SetMenu(winWindowRef, winMenuBar);
}

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
		window = SDL_CreateWindow("Chip-8 Emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
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

		HWND windowHandle = getWindowHandle();
		readyMenus(windowHandle);

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

	SDL_SetRenderDrawColor(renderer, pOnColor[0], pOnColor[1], pOnColor[2], 0xFF);

	for (int i = 0; i < 2048; i++) {
		if (screenData[i] != 0) {
			SDL_Rect rect;
			rect.y = (i / 64)*pixelHeight;
			rect.x = (i - ((i / 64) * 64))*pixelWidth;
			rect.w = pixelWidth;
			rect.h = pixelHeight;
			
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, pOnColor[0], pOnColor[1], pOnColor[2], 0x0);
}

std::string emuWindow::openFileDialog(){
	OPENFILENAME ofn;
	char szFile[260];
	HWND windowHandle;
	HANDLE fileHandle;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = getWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE) {
		return ofn.lpstrFile;
	}
	else {
		return nullptr;
	}
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

void emuWindow::changeScreenColors(Uint8 pOffR, Uint8 pOffG, Uint8 pOffB, Uint8 pOnR, Uint8 pOnG, Uint8 pOnB) {
	pOffColor[0] = pOffR;
	pOffColor[1] = pOffG;
	pOffColor[2] = pOffB;

	pOnColor[0] = pOnR;
	pOnColor[1] = pOnG;
	pOnColor[2] = pOnB;
}
