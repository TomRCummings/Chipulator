/*
Chip8: Implements and handles all Chip-8 specific functionality including: memory, registers, timers, stack, counters, opcode execution, drawing to the screen buffer, etc. Can be run independant of any other project
specific code if the calls to the logger are removed. Must be initialized and passed either a ROM or state array.
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <time.h>

class Chip8 {
public:
	void initialize();
	/*Loads ROM into memory location 512 onwards. Anything in the array past 3584 bytes will be ignored.*/
	void loadROM(unsigned char* program);
	/*Initializes Chip-8 with state. Bytes 0 to 4097 correspond to memory, 4098 to 4111 correspond to the V registers, 4112 to 4115 correspond to the I register and program counter,
	  4116 to 4147 make up the stack, bytes 4118 & 4119 are the stack pointer, 4150 to 4153 are the delay and sound timers, and 4154 to 6207 are the screen bytes (one byte per pixel).
	  Anything past byte 6207 will be ignored*/
	void loadState(unsigned char* state);
	unsigned char* getMemory();
	unsigned char* getRegisters();
	unsigned short getI();
	unsigned short* getIPointer();
	unsigned short getPC();
	unsigned short* getPCPointer();
	unsigned short* getOpCodePointer();
	unsigned short* getStack();
	unsigned short getStackPointer();
	unsigned short* getStackPointerPointer();
	unsigned char getDelayTimer();
	unsigned char* getDelayTimerPointer();
	unsigned char getSoundTimer();
	unsigned char* getSoundTimerPointer();
	unsigned char* getScreen();
	bool getDrawFlag();
	void setDrawFlag(bool setter);
	/*Set Chip-8 cpu to cycle, either on loop (if parameter is true) or only once.*/
	void runCycle(bool setLoop = true);
	/*Stop Chip-8 cpu cycling, either waiting for the current cycle to end (if parameter is true) or abruptly. Note that passing a false parameter to this function is unsafe and should
	  only be done at exit, reset, or load.*/
	void stopCycle(bool shouldWait = true);
	void cycle();
	bool isCPURunning();
	/*Will set the button represented by the parameter to true. Ignores any parameter that is not between 0 and 15.*/
	void updateInput(int buttonPressed);
	/*Sets a new CPU rate. Change will not take effect until the next time the cpu is started again.*/
	void setCPURate(int newRate);
	int getCPURate();

private:
	//CPU frequency
	float cpuRate = 800;
	//Display and timer update rate
	float updateRate = 60;
	//Flag to report if CPU is cycling
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
	//16-bit register (lowest 12 bits are used, mostly for memory addresses)
	unsigned short I;
	//Screen array (64x32 pixels)
	unsigned char screen[2048];
	bool drawFlag;
	//Key state (Chip-8 used hexadecimal keypad)
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

	void decrementTimers();

};