#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_MENU_OPEN(cMain::onMenuOpen)
	EVT_MENU_CLOSE(cMain::onMenuClose)
	EVT_KEY_DOWN(cMain::onKeyDown)
	EVT_MENU(openROMID, cMain::onOpenROM)
	EVT_MENU(resetID, cMain::onReset)
	EVT_MENU(pauseID, cMain::onPause)
	EVT_MENU(saveStateID, cMain::onSaveState)
	EVT_MENU(loadStateID, cMain::onLoadState)
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
	EVT_MENU(changeKeysID, cMain::onChangeKeys)
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
	m_pFile->AppendCheckItem(pauseID, _T("Pause"));
	m_pFile->AppendSeparator();
	m_pFile->Append(saveStateID, _T("Save State"));
	m_pFile->Append(loadStateID, _T("Load State"));
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

	//Create control menu
	m_pControls = new wxMenu();
	m_pControls->Append(changeKeysID, _T("Customize Controls"));

	//Create help menu
	m_pHelp = new wxMenu();
	m_pHelp->Append(aboutID, _T("About"));

	//Build and show menu bar on fram
	m_pMenuBar->Append(m_pFile, _T("File"));
	m_pMenuBar->Append(m_pGraphics, _T("Graphics"));
	m_pMenuBar->Append(m_pSound, _T("Sound"));
	m_pMenuBar->Append(m_pControls, _T("Controls"));
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

	//Set minimum size for and show wxWidgets
	this->SetMinSize(wxSize(640, 320));
	Show();

	//Load settings from config file
	parseConfigFile();

	//Set timer going
	lastTime = std::chrono::steady_clock::now();
}

cMain::~cMain() {
}

void cMain::onMenuOpen(wxMenuEvent& evt) {
	if (theChip8.isCPURunning()) {
		theChip8.stopCycle();
		soundMaker.setSoundOn(false);
	}

	evt.Skip();
}

void cMain::onMenuClose(wxMenuEvent& evt) {
	if (!theChip8.isCPURunning() && runChip8) {
		theChip8.runCycle();
	}

	evt.Skip();
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

	runChip8 = true;
	m_pFile->FindItem(pauseID)->Check(false);

	evt.Skip();
}

void cMain::onReset(wxCommandEvent& evt) {
	theChip8.initialize();
	theChip8.loadROM(chip8ROM);

	m_pFile->FindItem(pauseID)->Check(false);

	evt.Skip();
}

void cMain::onPause(wxCommandEvent& evt) {
	if (theChip8.isCPURunning()) {
		theChip8.stopCycle();
		runChip8 = false;
	}
	else {
		theChip8.runCycle();
		runChip8 = true;
	}

	evt.Skip();
}

void cMain::onSaveState(wxCommandEvent& evt) {
	bool wasRunning = theChip8.isCPURunning();
	if (wasRunning) {
		theChip8.stopCycle();
	}

	wxFileDialog
		saveFileDialog(this, _("Save Chip-8 state file"), "", "", "c8s files (*.c8s)|*.c8s",
			 wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	std::string filePathString = saveFileDialog.GetPath().ToStdString();
	saveChip8State(filePathString);

	if (wasRunning) {
		theChip8.runCycle();
	}

	evt.Skip();
}

void cMain::onLoadState(wxCommandEvent& evt) {
	wxFileDialog
		loadFileDialog(this, _("Load Chip-8 state file"), "", "", "c8s files (*.c8s)|*.c8s",
			wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (loadFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	std::string filePathString = loadFileDialog.GetPath().ToStdString();
	loadChip8State(filePathString);

	theChip8.setDrawFlag(true);
	theChip8.runCycle();

	evt.Skip();
}

void cMain::onExit(wxCommandEvent& evt) {
	theChip8.stopCycle();
	this->Close();

	evt.Skip();
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

	evt.Skip();
}

void cMain::onChangeWaveType(wxCommandEvent& evt) {
	if (evt.GetId() == sineID) {
		soundMaker.setWaveType(0);
	}
	else if (evt.GetId() == squareID) {
		soundMaker.setWaveType(1);
	}

	evt.Skip();
}

void cMain::onChangeWaveNote(wxCommandEvent& evt) {
	wxNumberEntryDialog dialog(this, wxT("Enter a frequency in Hz."), wxT(""), wxT("Change wave tone"), 440, 55, 1760);

	if (dialog.ShowModal() == wxID_OK) {
		soundMaker.setWaveTone(dialog.GetValue());
	}

	evt.Skip();
}

void cMain::onMute(wxCommandEvent& evt) {
	soundMaker.setMuteOn();

	evt.Skip();
}

void cMain::onChangeKeys(wxCommandEvent& evt)
{
	std::map<int, int> changedMap(keybindings);

	ControlPicker controlDialog(this, -1, wxT("Configure controls"), changedMap, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxWANTS_CHARS);

	if (controlDialog.ShowModal() == wxID_OK) {
		keybindings = changedMap;
	}

	evt.Skip();
}

void cMain::onAbout(wxCommandEvent& evt) {
	wxAboutDialogInfo info;
	info.SetName(_("Chip-8 Emulator"));
	info.SetVersion(_("0.0.0"));
	info.SetDescription(_("This program interprets and runs Chip-8 ROMS!"));
	info.SetCopyright(wxT("MIT License Tom Cummings cummings.t287@gmail.com"));
	wxAboutBox(info);

	evt.Skip();
}

void cMain::onKeyDown(wxKeyEvent& evt) {
	wxChar uc = evt.GetUnicodeKey();
	if (uc != WXK_NONE) {
		if (uc == keybindings[0]) {
			theChip8.updateInput(0);
		}
		else if (uc == keybindings[1]) {
			theChip8.updateInput(1);
		}
		else if (uc == keybindings[2]) {
			theChip8.updateInput(2);
		}
		else if (uc == keybindings[3]) {
			theChip8.updateInput(3);
		}
		else if (uc == keybindings[4]) {
			theChip8.updateInput(4);
		}
		else if (uc == keybindings[5]) {
			theChip8.updateInput(5);
		}
		else if (uc == keybindings[6]) {
			theChip8.updateInput(6);
		}
		else if (uc == keybindings[7]) {
			theChip8.updateInput(7);
		}
		else if (uc == keybindings[8]) {
			theChip8.updateInput(8);
		}
		else if (uc == keybindings[9]) {
			theChip8.updateInput(9);
		}
		else if (uc == keybindings[10]) {
			theChip8.updateInput(10);
		}
		else if (uc == keybindings[11]) {
			theChip8.updateInput(11);
		}
		else if (uc == keybindings[12]) {
			theChip8.updateInput(12);
		}
		else if (uc == keybindings[13]) {
			theChip8.updateInput(13);
		}
		else if (uc == keybindings[14]) {
			theChip8.updateInput(14);
		}
		else if (uc == keybindings[15]) {
			theChip8.updateInput(15);
		}
	}

	//evt.Skip();
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

void cMain::parseConfigFile() {
	std::ifstream configFile;
	std::string line;
	std::string key;
	std::string value;
	int uniValue;

	INFO << "Opening config file...";

	configFile.open("config.txt", std::ios::in);

	if (configFile) {
		INFO << "Config file opened successfully...";
	}
	else {
		INFO << "Config file not opened...";
	}

	while (std::getline(configFile, line)) {
		if (line.find("//") != 0 && line.length() > 0) {
			std::size_t pos = line.find(":");
			key = line.substr(0, pos);
			value = line.substr(pos + 1);

			std::size_t found = value.find_first_of(" ");

			while (found != std::string::npos) {
				if (found != 0) {
					value = value.substr(0, found);
				}
				else {
					value = value.substr(1);
				}
				found = value.find_first_of(" ");
			}

			
			uniValue = std::stoi(value, nullptr);

			if (key == "ZERO") {
				keybindings.insert(std::pair<int, int>(0, uniValue));
			}
			else if (key == "ONE") {
				keybindings.insert(std::pair<int, int>(1, uniValue));
			}
			else if (key == "TWO") {
				keybindings.insert(std::pair<int, int>(2, uniValue));
			}
			else if (key == "THREE") {
				keybindings.insert(std::pair<int, int>(3, uniValue));
			}
			else if (key == "FOUR") {
				keybindings.insert(std::pair<int, int>(4, uniValue));
			}
			else if (key == "FIVE") {
				keybindings.insert(std::pair<int, int>(5, uniValue));
			}
			else if (key == "SIX") {
				keybindings.insert(std::pair<int, int>(6, uniValue));
			}
			else if (key == "SEVEN") {
				keybindings.insert(std::pair<int, int>(7, uniValue));
			}
			else if (key == "EIGHT") {
				keybindings.insert(std::pair<int, int>(8, uniValue));
			}
			else if (key == "NINE") {
				keybindings.insert(std::pair<int, int>(9, uniValue));
			}
			else if (key == "A") {
				keybindings.insert(std::pair<int, int>(10, uniValue));
			}
			else if (key == "B") {
				keybindings.insert(std::pair<int, int>(11, uniValue));
			}
			else if (key == "C") {
				keybindings.insert(std::pair<int, int>(12, uniValue));
			}
			else if (key == "D") {
				keybindings.insert(std::pair<int, int>(13, uniValue));
			}
			else if (key == "E") {
				keybindings.insert(std::pair<int, int>(14, uniValue));
			}
			else if (key == "F") {
				keybindings.insert(std::pair<int, int>(15, uniValue));
			}
		}
	}
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

void cMain::saveChip8State(std::string filename) {
	std::ofstream file;
	file.open(filename.c_str(), std::ios::binary);
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
	unsigned char* machineScreen = theChip8.getScreen();
	file.write((char*)&machineMemory[0], 4096);
	file.write((char*)&machineRegisters[0], 16);
	file.write((char*)&machineI, sizeof(machineI));
	file.write((char*)&machinePC, sizeof(machinePC));
	file.write((char*)&machineStack[0], 16*sizeof(machineI));
	file.write((char*)&machineSP, sizeof(machineSP));
	file.write((char*)&machineDT, sizeof(machineDT));
	file.write((char*)&machineST, sizeof(machineST));
	file.write((char*)&machineScreen[0], 2048);
	file.close();
	INFO << "Wrote state to file.";
}

void cMain::loadChip8State(std::string filename) {
	std::ifstream file(filename.c_str(), std::ios::binary);

	unsigned char machineState[6208];
	for (int i = 0; i < 6208; i++) {
		machineState[i] = 0;
	}

	file.read((char*)&machineState[0], 6208);
	file.close();

	theChip8.stopCycle();
	theChip8.initialize();
	theChip8.loadState(machineState);
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
	theChip8.setDrawFlag(false);
}