#include "cMain.h"

const wxChar ZERO = '0';
const wxChar ONE = '1';
const wxChar TWO = '2';
const wxChar THREE = '3';
const wxChar FOUR = '4';
const wxChar FIVE = '5';
const wxChar SIX = '6';
const wxChar SEVEN = '7';
const wxChar EIGHT = '8';
const wxChar NINE = '9';
const wxChar A = 65;
const wxChar B = 66;
const wxChar C = 67;
const wxChar D = 68;
const wxChar E = 69;
const wxChar F = 70;

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_KEY_DOWN(cMain::onKeyDown)
	EVT_MENU(openROMID, cMain::onOpenROM)
	EVT_MENU(resetID, cMain::onReset)
	EVT_MENU(exitID, cMain::onExit)
	EVT_MENU(gPID, cMain::onChangeScreenColors)
	EVT_MENU(aPID, cMain::onChangeScreenColors)
	EVT_MENU(wOBID, cMain::onChangeScreenColors)
	EVT_MENU(bOWID, cMain::onChangeScreenColors)
	EVT_MENU(ccPixelID, cMain::onChangeScreenColors)
	EVT_MENU(ccBackID, cMain::onChangeScreenColors)
	EVT_MENU(sineID, cMain::onChangeWaveType)
	EVT_MENU(squareID, cMain::onChangeWaveType)
	EVT_MENU(noteID, cMain::onChangeWaveNote)
	EVT_MENU(muteID, cMain::onMute)
	EVT_MENU(aboutID, cMain::onAbout)
	EVT_IDLE(cMain::onIdle)
wxEND_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Chip-8 Emulator", wxPoint(30, 30), wxSize(640, 320)) {

	//Chip-8 setup
	theChip8.initialize();
	chip8ROMSize = 3584;

	//Create menu bar
	m_pMenuBar = new wxMenuBar();

	//Create file menu
	m_pFile = new wxMenu();
	m_pFile->Append(openROMID, _T("Open ROM"));
	m_pFile->Append(resetID, _T("Reset"));
	m_pFile->AppendSeparator();
	m_pFile->Append(exitID, _T("Exit"));

	//Create graphics menu
	m_pGraphics = new wxMenu();
	m_pGraphics->Append(gPID, _T("Green Phoshor"));
	m_pGraphics->Append(aPID, _T("Amber Phoshor"));
	m_pGraphics->Append(wOBID, _T("White on Black"));
	m_pGraphics->Append(bOWID, _T("Black on White"));
	m_pGraphics->AppendSeparator();
	m_pGraphics->Append(ccPixelID, _T("Custom Pixel Color"));
	m_pGraphics->Append(ccBackID, _T("Custom Background Color"));

	//Create sound menu
	m_pSound = new wxMenu();
	m_pWaveType = new wxMenu();
	m_pWaveType->Append(sineID, _T("Sine Wave"));
	m_pWaveType->Append(squareID, _T("Square Wave"));
	m_pSound->AppendSubMenu(m_pWaveType, _T("Change Wave"));
	m_pSound->Append(noteID, _T("Change Note"));
	m_pSound->AppendCheckItem(muteID, _T("Mute"));

	//Create help menu
	m_pHelp = new wxMenu();
	m_pHelp->Append(aboutID, _T("About"));

	//Build and show menu bar on fram
	m_pMenuBar->Append(m_pFile, _T("File"));
	m_pMenuBar->Append(m_pGraphics, _T("Graphics"));
	m_pMenuBar->Append(m_pSound, _T("Sound"));
	m_pMenuBar->Append(m_pHelp, _T("Help"));
	SetMenuBar(m_pMenuBar);

	//SDL window & renderer initialization
	bool sdlInitialized = true;
	sdlWindow = SDL_CreateWindowFrom((void*)this->GetHandle());
	if (sdlWindow == NULL) {
		//LOGGING HERE *****
		sdlInitialized = false;
	}
	else {
		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
		if (sdlRenderer == NULL) {
			//LOGGING HERE *****
			sdlInitialized = false;
		}
		else {
			//LOGGING HERE *****
			SDL_SetRenderDrawColor(sdlRenderer, 0x0, 0x0, 0x0, 0x0);
		}
	}

	//Sound initialization
	soundMaker.initializeAudio();

	this->SetMinSize(wxSize(640, 320));

	Show();

	lastTime = std::chrono::steady_clock::now();

}

cMain::~cMain() {
}

void cMain::onOpenROM(wxCommandEvent& evt) {

	INFO << "Open ROM event called...";

	wxFileDialog
		openFileDialog(this, _("Open Chip-8 ROM"), "", "",
			"", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	//Feed file path to load ROM function
	std::string filePathString = openFileDialog.GetPath().ToStdString();

	INFO << "File path chosen: " << filePathString;

	loadROM(filePathString);
}

void cMain::onReset(wxCommandEvent& evt) {

}

void cMain::onExit(wxCommandEvent& evt) {
	this->Close();
}

void cMain::onChangeScreenColors(wxCommandEvent& evt) {
	if (evt.GetId() == gPID) {
		pColor[0] = 51;
		pColor[1] = 255;
		pColor[2] = 0;
		bColor[0] = 0;
		bColor[1] = 0;
		bColor[2] = 0;
	}
	else if (evt.GetId() == aPID) {
		pColor[0] = 255;
		pColor[1] = 176;
		pColor[2] = 0;
		bColor[0] = 0;
		bColor[1] = 0;
		bColor[2] = 0;
	}
	else if (evt.GetId() == wOBID) {
		pColor[0] = 255;
		pColor[1] = 255;
		pColor[2] = 255;
		bColor[0] = 0;
		bColor[1] = 0;
		bColor[2] = 0;
	}
	else if (evt.GetId() == bOWID) {
		pColor[0] = 0;
		pColor[1] = 0;
		pColor[2] = 0;
		bColor[0] = 255;
		bColor[1] = 255;
		bColor[2] = 255;
	}
	else if (evt.GetId() == ccPixelID) {
		wxColourData data;
		data.SetChooseFull(true);
		wxColourDialog dialog(this, &data);

		if (dialog.ShowModal() == wxID_OK) {
			wxColourData retData = dialog.GetColourData();
			wxColour col = retData.GetColour();

			pColor[0] = col.Red();
			pColor[1] = col.Green();
			pColor[2] = col.Blue();
		}
	}
	else if (evt.GetId() == ccBackID) {
		wxColourData data;
		data.SetChooseFull(true);
		wxColourDialog dialog(this, &data);

		if (dialog.ShowModal() == wxID_OK) {
			wxColourData retData = dialog.GetColourData();
			wxColour col = retData.GetColour();

			bColor[0] = col.Red();
			bColor[1] = col.Green();
			bColor[2] = col.Blue();
		}
	}
}

void cMain::onChangeWaveType(wxCommandEvent& evt) {
	if (evt.GetId() == sineID) {
		soundMaker.setWaveType(0);
	}
	else if (evt.GetId() == squareID) {
		soundMaker.setWaveType(1);
	}
}

void cMain::onChangeWaveNote(wxCommandEvent& evt) {
	wxNumberEntryDialog dialog(this, wxT("Enter a frequency in Hz."), wxT(""), wxT("Change wave tone"), 440, 55, 1760);

	if (dialog.ShowModal() == wxID_OK) {
		soundMaker.setWaveTone(dialog.GetValue());
	}
}

void cMain::onMute(wxCommandEvent& evt) {
	soundMaker.setMuteOn();
}

void cMain::onAbout(wxCommandEvent& evt) {
	wxAboutDialogInfo info;
	info.SetName(_("Chip-8 Emulator"));
	info.SetVersion(_("0.0.0"));
	info.SetDescription(_("This program interprets and runs Chip-8 ROMS!"));
	info.SetCopyright(wxT("MIT License Tom Cummings cummings.t287@gmail.com"));
	wxAboutBox(info);
}

void cMain::onKeyDown(wxKeyEvent& evt) {
	wxChar uc = evt.GetUnicodeKey();
	if (uc != WXK_NONE) {
		switch (uc) {
		case ZERO:
			theChip8.updateInput(0);
			INFO << "Zero key was pressed down";
			break;
		case ONE:
			theChip8.updateInput(1);
			break;
		case TWO:
			theChip8.updateInput(2);
			break;
		case THREE:
			theChip8.updateInput(3);
			break;
		case FOUR:
			theChip8.updateInput(4);
			break;
		case FIVE:
			theChip8.updateInput(5);
			break;
		case SIX:
			theChip8.updateInput(6);
			break;
		case SEVEN:
			theChip8.updateInput(7);
			break;
		case EIGHT:
			theChip8.updateInput(8);
			break;
		case NINE:
			theChip8.updateInput(9);
			break;
		case A:
			theChip8.updateInput(10);
			break;
		case B:
			theChip8.updateInput(11);
			break;
		case C:
			theChip8.updateInput(12);
			break;
		case D:
			theChip8.updateInput(13);
			break;
		case E:
			theChip8.updateInput(14);
			break;
		case F:
			theChip8.updateInput(15);
			break;
		default:
			std::cout << uc << std::endl;
		}
	}
}

void cMain::onIdle(wxIdleEvent& evt) {
	if (accumulator.count() < renderRate) {
		accumulator += (std::chrono::steady_clock::now() - lastTime);
		lastTime = std::chrono::steady_clock::now();
	}
	else {
		//Render
		if (theChip8.getDrawFlag()) {
			drawScreen();
		}

		//Turn sound on or off
		if (theChip8.getSoundTimer() > 0) {
			soundMaker.setSoundOn(true);
		}
		else {
			soundMaker.setSoundOn(false);
		}

		accumulator = std::chrono::milliseconds::zero();
		lastTime = std::chrono::steady_clock::now();
	}
	evt.RequestMore();
}

void cMain::loadROM(std::string filename) {
	//Zero ROM array
	INFO << "Opening ROM...";
	for (int i = 0; i < chip8ROMSize; i++) {
		chip8ROM[i] = 0;
	}

	INFO << "Zeroed ROM array...";

	//Open file stream to specified file
	std::ifstream inf(filename.c_str(), std::ios::binary);

	INFO << "Opened file...";

	if (inf) {
		//Read in bytes from file to buffer
		inf.read((char*)&chip8ROM[0], chip8ROMSize);
		inf.close();

		INFO << "Read file into ROM array...";

		theChip8.initialize();
		theChip8.loadROM(chip8ROM);

		INFO << "Loaded ROM into Chip-8...";

		theChip8.runCycle();
	}
	else {
		ERRORLOG << "Not able to open chosen file...";
	}
}

void cMain::saveChip8State() {
	std::ofstream file;
	file.open("chip8state.c8state", std::ios::binary);
	unsigned char* machineMemory = theChip8.getMemory();
	unsigned char* machineRegisters = theChip8.getRegisters();
	unsigned short machineI = theChip8.getI();
	std::cout << machineI << std::endl;
	unsigned short machinePC = theChip8.getPC();
	std::cout << machinePC << std::endl;
	unsigned short* machineStack = theChip8.getStack();
	unsigned short machineSP = theChip8.getStackPointer();
	unsigned short machineDT = theChip8.getDelayTimer();
	unsigned short machineST = theChip8.getSoundTimer();
	file.write((char*)&machineMemory[0], 4096);
	file.write((char*)&machineRegisters[0], 16);
	file.write((char*)&machineI, sizeof(machineI));
	file.write((char*)&machinePC, sizeof(machinePC));
	file.write((char*)&machineStack, sizeof(machineStack));
	file.write((char*)&machineSP, sizeof(machineSP));
	file.write((char*)&machineDT, sizeof(machineDT));
	file.write((char*)&machineST, sizeof(machineST));
	file.close();
}

void cMain::drawScreen() {
	unsigned char* screenData = theChip8.getScreen();

	SDL_RenderClear(sdlRenderer);
	int windowWidth = 0;
	int windowHeight = 0;
	SDL_GetWindowSize(sdlWindow, &windowWidth, &windowHeight);
	const int pixelWidth = windowWidth / 64;
	const int pixelHeight = windowHeight / 32;

	SDL_SetRenderDrawColor(sdlRenderer, pColor[0], pColor[1], pColor[2], 0x0);

	for (int i = 0; i < 2048; i++) {
		if (screenData[i] != 0) {
			SDL_Rect rect;
			rect.y = (i / 64) * pixelHeight;
			rect.x = (i - ((i / 64) * 64)) * pixelWidth;
			rect.w = pixelWidth;
			rect.h = pixelHeight;

			SDL_RenderFillRect(sdlRenderer, &rect);
		}
	}
	SDL_RenderPresent(sdlRenderer);
	SDL_SetRenderDrawColor(sdlRenderer, bColor[0], bColor[1], bColor[2], 0x0);
}