#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <Windows.h>
#include <SDL.h>
#include <chrono>
#include "chip8.h"
#include "logger.h"
#include "emuWindow.h"
#include "beeper.h"

//WinAPI button IDs
enum buttonIDs { loadRomID, resetID, exitID, controlsID, greenPhosphorID, amberPhosphorID, whiteOnBlackID, blackOnWhiteID, customColorPixelID, customColorBackID, sineWaveID, squareWaveID, waveToneID, muteSoundID, aboutID };

//Keybindings
const int ZERO = SDLK_0;
const int ONE = SDLK_1;
const int TWO = SDLK_2;
const int THREE = SDLK_3;
const int FOUR = SDLK_4;
const int FIVE = SDLK_5;
const int SIX = SDLK_6;
const int SEVEN = SDLK_7;
const int EIGHT = SDLK_8;
const int NINE = SDLK_9;
const int A = SDLK_a;
const int B = SDLK_b;
const int C = SDLK_c;
const int D = SDLK_d;
const int E = SDLK_e;
const int F = SDLK_f;

//Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

//Timing constants
const float updateRate = 60;
const float cpuRate = 800;
const float cpuTicksTilUpdate = cpuRate / updateRate;
const float msPerTick = (1.0 / cpuRate) * 1000.0;

//Timing counters
float currentCPUTicks = 0;
float countCPUCyclesForDraw = 0;

//Clocks
std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();
std::chrono::duration<float, std::milli> accumulator{};

//Chip-8 emulation core class
chip8 theChip8;

//Window handling class
EmuWindow mainWindow;

//Sound handling class
Beeper soundMaker;

//Save state function (needs work!)
void saveChip8State(chip8 machine);

//Load ROM function
void readFromRom(std::string filename, unsigned char* dataBuffer, size_t fileSize);

int main( int argc, char* args[]) {

	//SDL initialization
	mainWindow.initialize(SCREEN_WIDTH, SCREEN_HEIGHT);
	soundMaker.initializeAudio();

	//Event holder
	SDL_Event e;
	//Allow event holder to consume events from Windows
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

	INFO << "Main() started";

	theChip8.initialize();
	std::string romFilePath = "C:/Users/Ryan/Documents/Projects/chip8emu/ROMS/Pong.ch8";
	//Max number of bytes in a Chip-8 ROM
	const size_t fileSize = 3586;
	//Create program buffer
	unsigned char dataBuffer[fileSize] = { 0 };
	readFromRom(romFilePath, dataBuffer, fileSize);
	theChip8.loadROM(dataBuffer);
	//saveChip8State(theChip8);

	for (;;) {

		//Handle window closure
		if (mainWindow.getQuitFlag()) {
			mainWindow.close();
			break;
		}

		//Handle input using SDL event handler
		while (SDL_PollEvent(&e) > 0) {
			//INFO << "Polling for events...";
			switch (e.type) {
			case SDL_QUIT:
				mainWindow.setQuitFlag(true);
				break;
			case SDL_KEYDOWN:
				//INFO << "Key down from: " << e.key.keysym.sym;
				switch (e.key.keysym.sym) {
				case ZERO:
					theChip8.updateInput(0);
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
					//INFO << "key not bound";
					break;
				}
				break;
			case SDL_SYSWMEVENT:
				if (e.syswm.msg->msg.win.msg == WM_COMMAND) {
					if (LOWORD(e.syswm.msg->msg.win.wParam) == exitID) {
						mainWindow.setQuitFlag(true);
					}
					else if (LOWORD(e.syswm.msg->msg.win.wParam) == loadRomID) {
						//Open and get string from file choose dialog
						romFilePath = mainWindow.openFileDialog();
						//Re-initialize Chip-8
						theChip8.initialize();
						//Read ROM from file
						readFromRom(romFilePath, dataBuffer, fileSize);
						//Load ROM to Chip-8
						theChip8.loadROM(dataBuffer);
					}
					else if (LOWORD(e.syswm.msg->msg.win.wParam) == resetID) {
						//Re-initialize Chip-8
						theChip8.initialize();
						//Load ROM to Chip-8
						theChip8.loadROM(dataBuffer);
					}
					else if (LOWORD(e.syswm.msg->msg.win.wParam) == greenPhosphorID || LOWORD(e.syswm.msg->msg.win.wParam) == amberPhosphorID || LOWORD(e.syswm.msg->msg.win.wParam) == whiteOnBlackID || LOWORD(e.syswm.msg->msg.win.wParam) == blackOnWhiteID) {
						int colorID = LOWORD(e.syswm.msg->msg.win.wParam);
						mainWindow.changeScreenByID(colorID);
					}
					else if (LOWORD(e.syswm.msg->msg.win.wParam) == customColorPixelID) {
						mainWindow.changeScreenColorCustom(true);
					}
					else if (LOWORD(e.syswm.msg->msg.win.wParam) == customColorBackID) {
						mainWindow.changeScreenColorCustom(false);
					}
					else if (LOWORD(e.syswm.msg->msg.win.wParam) == sineWaveID) {
						soundMaker.setWaveType(0);
					}
					else if (LOWORD(e.syswm.msg->msg.win.wParam) == squareWaveID) {
						soundMaker.setWaveType(1);
					}
					else if (LOWORD(e.syswm.msg->msg.win.wParam) == muteSoundID) {
						soundMaker.setMuteOn();
					}
				}
				break;
			/*
			case SDL_KEYUP:
				INFO << "Key up from: " << e.key.keysym.sym;
				switch (e.key.keysym.sym) {
				case ZERO:
					theChip8.updateInput('0');
					break;
				case ONE:
					theChip8.updateInput('1');
					break;
				case TWO:
					theChip8.updateInput('2');
					break;
				case THREE:
					theChip8.updateInput('3');
					break;
				case FOUR:
					theChip8.updateInput('4');
					break;
				case FIVE:
					theChip8.updateInput('5');
					break;
				case SIX:
					theChip8.updateInput('6');
					break;
				case SEVEN:
					theChip8.updateInput('7');
					break;
				case EIGHT:
					theChip8.updateInput('8');
					break;
				case NINE:
					theChip8.updateInput('9');
					break;
				case A:
					theChip8.updateInput('a');
					break;
				case B:
					theChip8.updateInput('b');
					break;
				case C:
					theChip8.updateInput('c');
					break;
				case D:
					theChip8.updateInput('d');
					break;
				case E:
					theChip8.updateInput('e');
					break;
				case F:
					theChip8.updateInput('f');
					break;
				default:
					INFO << "key not bound";
				}
				break;
			*/
			}
		}

		//Clock for capping cycles per second
		accumulator += (std::chrono::steady_clock::now() - lastTime);

		if (accumulator.count() >= msPerTick) {
			currentCPUTicks++;
			accumulator = std::chrono::milliseconds::zero();
		}
		else {
			lastTime = std::chrono::steady_clock::now();
			continue;
		}

		//Main emulation cycle
		theChip8.cycle();
		//Counter to sync screen draws with FPS
		countCPUCyclesForDraw++;
		if (countCPUCyclesForDraw >= cpuTicksTilUpdate) {
			//Only draw if we need to
			if (theChip8.getDrawFlag()) {
				mainWindow.drawScreen(theChip8.getScreen());
				theChip8.setDrawFlag(false);
			}
			if (theChip8.getSoundTimer() > 0) {
				soundMaker.setSoundOn(true);
			}
			else {
				soundMaker.setSoundOn(false);
			}
			theChip8.decrementTimers();
			countCPUCyclesForDraw = 0;
		}
		lastTime = std::chrono::steady_clock::now();
	}
	return 0;
}

void saveChip8State(chip8 machine) {
	std::ofstream file;
	file.open("chip8state.c8", std::ios::binary);
	unsigned char* machineMemory = machine.getMemory();
	unsigned char* machineRegisters = machine.getRegisters();
	unsigned short machineI = machine.getI();
	std::cout << machineI << std::endl;
	unsigned short machinePC = machine.getPC();
	std::cout << machinePC << std::endl;
	unsigned short* machineStack = machine.getStack();
	unsigned short machineSP = machine.getStackPointer();
	unsigned short machineDT = machine.getDelayTimer();
	unsigned short machineST = machine.getSoundTimer();
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

void readFromRom(std::string filename, unsigned char* dataBuffer, size_t fileSize) {
	//Open file stream to specified file
	std::ifstream inf(filename.c_str(), std::ios::binary);
	int i = 0;

	if (inf) {
		//Read in bytes from file to buffer
		i = 1;
		inf.read((char*)&dataBuffer[0], fileSize);
	}
}