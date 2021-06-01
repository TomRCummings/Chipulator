#pragma once

#include <logger.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <time.h>
#include <wx/event.h>

wxDECLARE_EVENT(DRAW_SCREEN, wxCommandEvent);

class chip8 {
	//CPU frequency
	float cpuRate = 800;
	//Display and timer update rate
	float updateRate = 60;
	//Flag to tell if CPU is cycling
	bool isRunning = false;
	//Flag to tell CPU to cycle continuously
	bool shouldLoop = false;
	//Thread to contain CPU cycling
	std::thread cpuThread;
	//The current opcode to be executed
	unsigned short opcode;
	//4KB memory array
	unsigned char memory[4096];
	//16 8-bit registers
	unsigned char V[16];
	//16-bit register (stores memory addresses, so rightmost 12 bit are used)
	unsigned short I;
	//Screen array (64x32 pixels)
	unsigned char screen[2048];
	bool drawFlag;
	//Key state (chip-8 used hexadecimal keypad)
	bool key[16];
	//Program counter
	unsigned short pc;
	//Stack
	unsigned short stack[16];
	//Stack pointer
	unsigned short stackPointer;
	//Timers
	unsigned char delayTimer;
	unsigned char soundTimer;
	//Chip-8 native font
	unsigned char chip8FontSet[80] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

public:
	void initialize();
	void loadROM(unsigned char* program);
	unsigned char* getMemory();
	unsigned char* getRegisters();
	unsigned short getI();
	unsigned short getPC();
	unsigned short* getStack();
	unsigned short getStackPointer();
	unsigned short getDelayTimer();
	unsigned short getSoundTimer();
	unsigned char* getScreen();
	bool getDrawFlag();
	void setDrawFlag(bool setter);
	void decrementTimers();
	void cycle();
	void runCycle(bool setLoop = true);
	void stopCycle(bool shouldWait = true);
	bool isCPURunning();
	void updateInput(int buttonPressed);
};