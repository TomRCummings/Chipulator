#include "emuWindow.h"

//Screen color options
const Uint8 greenPhosphor[6] = { 0,0,0,51,255,0 };
const Uint8 amberPhosphor[6] = { 0,0,0,255,176,0 };
const Uint8 whiteOnBlack[6] = { 0,0,0,255,255,255 };
const Uint8 blackOnWhite[6] = { 255,255,255,0,0,0 };

//WinAPI button IDs
enum buttonIDs { loadRomID, resetID, exitID, controlsID, greenPhosphorID, amberPhosphorID, whiteOnBlackID, blackOnWhiteID, customColorPixelID, customColorBackID, beepSoundID, aboutID };

//WinAPI menu objects
static HMENU winMenuBar = NULL;
static HMENU winFile = NULL;
static HMENU winGraphics = NULL;
static HMENU winScreenColors = NULL;
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
	winScreenColors = CreateMenu();
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
	AppendMenu(winGraphics, MF_POPUP, (UINT_PTR)winScreenColors, "Screen Colors");
	AppendMenu(winScreenColors, MF_STRING, greenPhosphorID, "Green Phosphor");
	AppendMenu(winScreenColors, MF_STRING, amberPhosphorID, "Amber Phosphor");
	AppendMenu(winScreenColors, MF_STRING, whiteOnBlackID, "White On Black");
	AppendMenu(winScreenColors, MF_STRING, blackOnWhiteID, "Black On White");
	AppendMenu(winScreenColors, MF_STRING, customColorPixelID, "Custom Colors - Pixel");
	AppendMenu(winScreenColors, MF_STRING, customColorBackID, "Custom Colors - Background");

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

	SDL_SetRenderDrawColor(renderer, pOnColor[0], pOnColor[1], pOnColor[2], 0x0);

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
	SDL_SetRenderDrawColor(renderer, pOffColor[0], pOffColor[1], pOffColor[2], 0x0);
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

void emuWindow::changeScreenByID(int buttonID) {
	switch (buttonID) {
	case amberPhosphorID:
		changeScreenColors(amberPhosphor[0], amberPhosphor[1], amberPhosphor[2], amberPhosphor[3], amberPhosphor[4], amberPhosphor[5]);
		break;
	case greenPhosphorID:
		changeScreenColors(greenPhosphor[0], greenPhosphor[1], greenPhosphor[2], greenPhosphor[3], greenPhosphor[4], greenPhosphor[5]);
		break;
	case blackOnWhiteID:
		changeScreenColors(blackOnWhite[0], blackOnWhite[1], blackOnWhite[2], blackOnWhite[3], blackOnWhite[4], blackOnWhite[5]);
		break;
	case whiteOnBlackID:
		changeScreenColors(whiteOnBlack[0], whiteOnBlack[1], whiteOnBlack[2], whiteOnBlack[3], whiteOnBlack[4], whiteOnBlack[5]);
		break;
	}
}

void emuWindow::changeScreenColorCustom(bool pixel) {
	CHOOSECOLOR cc;
	static COLORREF crCustClr[16];

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = getWindowHandle();
	cc.lpCustColors = (LPDWORD)crCustClr;
	cc.rgbResult = RGB(0, 255, 0);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	ChooseColor(&cc);

	if (pixel) {
		changeScreenColors(pOffColor[0], pOffColor[1], pOffColor[2], GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult));
	}
	else {
		changeScreenColors(GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult), pOnColor[0], pOnColor[1], pOnColor[2]);
	}
}