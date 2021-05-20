#include "chip8.h"
#include "logger.h"
#include "emuWindow.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <Windows.h>
#include <SDL.h>
#include <chrono>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

const float updateRate = 60;
const float cpuRate = 600;
const float cpuTicksTilUpdate = cpuRate / updateRate;
const float msPerTick = (1.0 / cpuRate) * 1000.0;

float currentCPUTicks = 0;
float countCPUCyclesForDraw = 0;

std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();
std::chrono::duration<float, std::milli> accumulator{};

chip8 theChip8;

emuWindow mainWindow;

void saveChip8State(chip8 machine);
void readFromRom(std::string filename, unsigned char* dataBuffer, size_t fileSize);
void printScreenToConsole(unsigned char* screenBuffer, wchar_t* consoleBuffer);
unsigned char getInput();

int main( int argc, char* args[]) {
	mainWindow.initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	INFO << "Main() started";

	theChip8.initialize();
	std::string romFilePath = "C:/Users/Ryan/Documents/Projects/chip8emu/ROMS/heart_monitor.ch8";
	//Max number of bytes in a Chip-8 ROM
	const size_t fileSize = 3586;
	//Create program buffer
	unsigned char dataBuffer[fileSize] = { 0 };
	readFromRom(romFilePath, dataBuffer, fileSize);
	theChip8.loadROM(dataBuffer);
	//saveChip8State(theChip8);

	for (;;) {
		accumulator += (std::chrono::steady_clock::now() - lastTime);

		if (accumulator.count() >= msPerTick) {
			currentCPUTicks++;
			accumulator = std::chrono::milliseconds::zero();
		}
		else {
			lastTime = std::chrono::steady_clock::now();
			continue;
		}

		theChip8.cycle();
		countCPUCyclesForDraw++;
		if (countCPUCyclesForDraw >= cpuTicksTilUpdate) {
			if (theChip8.getDrawFlag()) {
				std::cout << std::string(50, '\n');
				mainWindow.drawScreen(theChip8.getScreen());
				theChip8.setDrawFlag(false);
			}
			theChip8.decrementTimers();
			theChip8.updateInput(getInput());
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

void printScreenToConsole(unsigned char* screenBuffer, wchar_t* consoleBuffer) {
	for (int i = 0; i < 2048; i++) {
		if (screenBuffer[i] != 0) {
			consoleBuffer[i] = char(254);
		}
		else {
			consoleBuffer[i] = '.';
		}
		consoleBuffer[nScreenHeight * nScreenWidth - 1] = '\0';
	}
}

unsigned char getInput() {
	if (GetKeyState('0') & 0x8000) {
		return 0x0;
	}
	if (GetKeyState('1') & 0x8000) {
		return 0x1;
	}
	if (GetKeyState('2') & 0x8000) {
		return 0x2;
	}
	if (GetKeyState('3') & 0x8000) {
		return 0x3;
	}
	if (GetKeyState('4') & 0x8000) {
		return 0x4;
	}
	if (GetKeyState('5') & 0x8000) {
		return 0x5;
	}
	if (GetKeyState('6') & 0x8000) {
		return 0x6;
	}
	if (GetKeyState('7') & 0x8000) {
		return 0x7;
	}
	if (GetKeyState('8') & 0x8000) {
		return 0x8;
	}
	if (GetKeyState('9') & 0x8000) {
		return 0x9;
	}
	if (GetKeyState('A') & 0x8000) {
		return 0xA;
	}
	if (GetKeyState('B') & 0x8000) {
		return 0xB;
	}
	if (GetKeyState('C') & 0x8000) {
		return 0xC;
	}
	if (GetKeyState('D') & 0x8000) {
		return 0xD;
	}
	if (GetKeyState('E') & 0x8000) {
		return 0xE;
	}
	if (GetKeyState('F') & 0x8000) {
		return 0xF;
	}
	return 0xFF;
}