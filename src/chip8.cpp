#include "chip8.h"

void Chip8::initialize() {
	srand(time(NULL));
	opcode = 0x0;
	I = 0x0;
	pc = 0x200;
	stackPointer = 0x0;
	delayTimer = 0x0;
	soundTimer = 0x0;
	drawFlag = false;
	for (int i = 0; i < 4096; i++) {
		if (i < 80) {
			memory[i] = chip8FontSet[i];
		}
		else {
			memory[i] = 0x0;
		}
		if (i < 2048) {
			screen[i] = false;
		}
		if (i < 16) {
			key[i] = false;
			stack[i] = 0x0;
			V[i] = 0x0;
		}
	}
}

void Chip8::loadROM(unsigned char* program) {
	for (int i = 512; i < 4096; i++) {
		memory[i] = program[i - 512];
	}
}

void Chip8::loadState(unsigned char* state) {
	for (int i = 0; i < 6208; i++) {
		if (i < 4096) {
			memory[i] = state[i];
		}
		else if (i < 4112) {
			V[i - 4096] = state[i];
		}
		else if (i == 4112) {
			I = (unsigned short)state[i];
		}
		else if (i == 4113) {
			I = I | (unsigned short)state[i];
		}
		else if (i == 4114) {
			pc = (unsigned short)state[i];
		}
		else if (i == 4115) {
			pc = pc | ((unsigned short)state[i] << 8);
		}
		else if (i < 4148) {
			if ((i % 2) == 0) {
				stack[(i - 4116) / 2] = (unsigned short)state[i];
			}
			else {
				stack[(i - 4116) / 2] = stack[(i - 4116) / 2] | ((unsigned short)state[i] << 8);
			}
		}
		else if (i < 4150) {
			if (i == 4149) {
				stackPointer = (unsigned short)state[i];
			}
			else {
				stackPointer = stackPointer | ((unsigned short)state[i] << 8);
			}
		}
		else if (i < 4152) {
			soundTimer = soundTimer | state[i];
		}
		else if (i < 4154) {
			delayTimer = delayTimer | state[i];
		}
		else {
			screen[i - 4154] = state[i];
		}
	}
}

unsigned char* Chip8::getMemory() {
	return memory;
}

unsigned char* Chip8::getRegisters() {
	return V;
}

unsigned short Chip8::getI() {
	return I;
}

unsigned short* Chip8::getIPointer()
{
	return &I;
}

unsigned short Chip8::getPC() {
	return pc;
}

unsigned short* Chip8::getPCPointer()
{
	return &pc;
}

unsigned short* Chip8::getOpCodePointer()
{
	return &opcode;
}

unsigned short* Chip8::getStack() {
	return stack;
}

unsigned short Chip8::getStackPointer() {
	return stackPointer;
}

unsigned short* Chip8::getStackPointerPointer()
{
	return &stackPointer;
}

unsigned char Chip8::getDelayTimer() {
	return delayTimer;
}

unsigned char* Chip8::getDelayTimerPointer()
{
	return &delayTimer;
}

unsigned char Chip8::getSoundTimer() {
	return soundTimer;
}

unsigned char* Chip8::getSoundTimerPointer()
{
	return &soundTimer;
}

unsigned char* Chip8::getScreen() {
	return screen;
}

bool Chip8::getDrawFlag() {
	return drawFlag;
}

void Chip8::setDrawFlag(bool setter) {
	drawFlag = setter;
}

void Chip8::decrementTimers() {
	if (delayTimer > 0) {
		delayTimer--;
	}
	if (soundTimer > 0) {
		soundTimer--;
	}
}

void Chip8::cycle() {
	//Timing constants
	const float cpuTicksTilUpdate = cpuRate / updateRate;
	const float msPerTick = (1.0 / cpuRate) * 1000.0;
	
	//Timing counters
	float currentCPUTicks = 0;
	float countCPUCyclesForDraw = 0;

	//Clocks
	std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();
	std::chrono::duration<float, std::milli> accumulator{};

	//Display variables
	unsigned short x;
	unsigned short y;
	unsigned short pixel;

	isRunning = true;

	do {
		//Get the current opcode from memory using the program counter
		opcode = memory[pc] << 8 | memory[pc + 1];
		//Decode and execute opcode in a switch statement
		switch (opcode & 0xF000) {
		case 0xA000: //0xAnnn: Set register I equal to nnn
			I = opcode & 0x0FFF;
			pc += 2;
			break;
		case 0xB000: //0xBnnn: Jump to location nnn + value of register 0
			pc = (opcode & 0x0FFF) + V[0];
			break;
		case 0xC000: //0xCxkk: Generate random byte, AND it with kk, and store in register x
			V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
			pc += 2;
			break;
		case 0xD000: //0xDxyn: Display n-byte sprite at (register x, register y), starting at memory location I
			x = V[(opcode & 0x0F00) >> 8];
			y = V[(opcode & 0x00F0) >> 4];
			V[0xF] = 0;
			for (int i = 0; i < (opcode & 0x000F); i++) {
				pixel = memory[I + i];
				for (int j = 0; j < 8; j++) {
					if ((pixel & (0x80 >> j)) != 0) {
						if (screen[(((x + j) % 64) + ((y + i) * 64)) % (64 * 32)] == 1) {
							V[0xF] = 1;
						}
						screen[(((x + j) % 64) + ((y + i) * 64)) % (64 * 32)] ^= 1;
					}
				}
			}
			drawFlag = true;
			pc += 2;
			break;
		case 0xE000:
			switch (opcode & 0x00FF) {
			case 0x009E: //0xEx9E: Skip next instruction if the key in register x is pressed
				if (key[V[(opcode & 0x0F00) >> 8]]) {
					pc += 4;
					for (int i = 0; i < 16; i++) {
						key[i] = false;
					}
				}
				else {
					pc += 2;
				}

				break;
			case 0x00A1: //0xExA1: Skip next instruction if the key in register x is not pressed
				if (!key[V[(opcode & 0x0F00) >> 8]]) {
					pc += 4;
				}
				else {
					pc += 2;
					for (int i = 0; i < 16; i++) {
						key[i] = false;
					}
				}
				break;
			}
			break;
		case 0xF000:
			switch (opcode & 0x00FF) {
			case 0x000A: //0xFx0A: Wait for key press, store key value in register x
				for (int i = 0; i < 16; i++) {
					if (key[i]) {
						V[(opcode & 0x0F00) >> 8] = i;
						pc += 2;
						for (int j = 0; j < 16; j++) {
							key[j] = false;
						}
					}
				}
				break;
			case 0x001E: //0xFx1E: Add I to register x and store result in I
				I = I + V[(opcode & 0x0F00) >> 8];
				pc += 2;
				break;
			case 0x0007: //0xFx07: Set register x to the delay timer value
				V[(opcode & 0x0F00) >> 8] = delayTimer;
				pc += 2;
				break;
			case 0x0015: //0xFx15: Set delay timer to the value in register x
				delayTimer = V[(opcode & 0x0F00) >> 8];
				pc += 2;
				break;
			case 0x0018: //0xFx18: Set sound timer to the value in register x
				soundTimer = V[(opcode & 0x0F00) >> 8];
				pc += 2;
				break;
			case 0x0029: //0xFx29: Value of I is set to the location of the sprite for the value of register x
				I = V[(opcode & 0x0F00) >> 8] * 5;
				pc += 2;
				break;
			case 0x0033: //0xFx33: Store BCD representation of value at register x in memory location I, I+1, and I+2
				memory[I] = V[(opcode & 0x0F00) >> 8] / 100; //100's digit at memory address I
				memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10; //10's digit at memory address I+1
				memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10; //1's digit at memory address I+2
				pc += 2;
				break;
			case 0x0055: //0xFx55: Starting at memory address I, puts values from registers 0 to x into memory
				for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
					memory[I + i] = V[i];
				}
				pc += 2;
				break;
			case 0x0065: //0xFx65: Starting at memory address I, puts values from memory into registers 0 to x
				for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
					V[i] = memory[I + i];
				}
				pc += 2;
				break;
			}
			break;
		case 0x1000: //0x1nnn: Jump to address nnn
			pc = opcode & 0x0FFF;
			break;
		case 0x2000: //0x2nnn: Go to subroutine at address nnn
			stack[stackPointer] = pc;
			stackPointer++;
			pc = opcode & 0x0FFF;
			break;
		case 0x3000: //0x3xkk: If register x = kk, skip next opcode
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
				pc += 4;
			}
			else {
				pc += 2;
			}
			break;
		case 0x4000: //0x4xkk: If register x != kk, skip next opcode
			if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
				pc += 2;
			}
			else {
				pc += 4;
			}
			break;
		case 0x5000: //0x5xy0: If register x = register y, skip next instruction
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
				pc += 4;
			}
			else {
				pc += 2;
			}
			break;
		case 0x6000: //0x6xkk: Set register x equal to kk
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			pc += 2;
			break;
		case 0x7000: //0x7xkk: Add kk to the value in register x and put that into register x
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] + (opcode & 0x00FF);
			pc += 2;
			break;
		case 0x8000:
			switch (opcode & 0x000F) {
			case 0x0000: //0x8xy0: Set register x = register y
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
				pc += 2;
				break;
			case 0x0001: //0x8xy1: Bitwise OR on the value of register x and y and stores the result in x
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
				pc += 2;
				break;
			case 0x0002: //0x8xy2: Bitwise AND on the value of register x and y and stores the result in x
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
				pc += 2;
				break;
			case 0x0003: //0x8xy3: Bitwise XOR on the value of register x and y and stores the result in x
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
				pc += 2;
				break;
			case 0x0004: //0x8xy4: Add the value of register x and y and stores the result in x
				if (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] > 0xFF) {
					V[0xF] = 1;
				}
				else {
					V[0xF] = 0;
				}
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
				pc += 2;
				break;
			case 0x0005: //0x8xy5: Subtracts register y from x and stores the result in x
				if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) {
					V[0xF] = 1;
				}
				else {
					V[0xF] = 0;
				}
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
				pc += 2;
				break;
			case 0x0006: //0x8xy6: Divide register x by 2
				if ((V[(opcode & 0x0F00) >> 8] & 0x0001) != 0) {
					V[0xF] = 1;
				}
				else {
					V[0xF] = 0;
				}
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] / 2;
				pc += 2;
				break;
			case 0x0007: //0x8xy7: Subtracts register x from y and stores the result in x
				if (V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4]) {
					V[0xF] = 1;
				}
				else {
					V[0xF] = 0;
				}
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
				pc += 2;
				break;
			case 0x000E: //0x8xyE: Multiply register x by 2
				if ((V[(opcode & 0x0F00) >> 8] >> 7) == 1) {
					V[0xF] = 1;
				}
				else {
					V[0xF] = 0;
				}
				V[(opcode & 0x0F00) >> 8] = 2 * V[(opcode & 0x0F00) >> 8];
				pc += 2;
				break;
			}
			break;
		case 0x9000: //0x9xy0: Skip next instruction if the value in register x != value in register y
			if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
				pc += 4;
			}
			else {
				pc += 2;
			}
			break;
		case 0x0000:
			switch (opcode & 0x00FF) {
			case 0x00E0: //0x00E0: Clear display
				for (int i = 0; i < 2048; i++) {
					screen[i] = 0;
				}
				drawFlag = true;
				pc += 2;
				break;

			case 0x00EE: //0x00EE: Return from subroutine
				stackPointer--;
				pc = stack[stackPointer];
				pc += 2;
				break;

			default: //0x0nnn: Deprecated (used to jump to nnn)
				pc += 2;
				break;
			}
			break;
		}
		currentCPUTicks++;

		if (currentCPUTicks >= cpuTicksTilUpdate) {
			decrementTimers();
			currentCPUTicks = 0;
		}

		
		
		//Start timing til next run
		accumulator += (std::chrono::steady_clock::now() - lastTime);
		lastTime = std::chrono::steady_clock::now();

		while (accumulator.count() < msPerTick) {
			accumulator += (std::chrono::steady_clock::now() - lastTime);
			lastTime = std::chrono::steady_clock::now();
		}

		//Reset counter for next cycle
		lastTime = std::chrono::steady_clock::now();
		accumulator = std::chrono::milliseconds::zero();

	} while (shouldLoop);

	isRunning = false;
}

void Chip8::runCycle(bool setLoop) {
	shouldLoop = false;
	if (cpuThread.joinable()) {
		cpuThread.join();
	}
	shouldLoop = setLoop;
	cpuThread = std::thread(&Chip8::cycle, this);
}

void Chip8::stopCycle(bool shouldWait) {
	shouldLoop = false;
	if (shouldWait) {
		if (cpuThread.joinable()) {
			cpuThread.join();
		}
	}
}

bool Chip8::isCPURunning()
{
	return isRunning;
}

void Chip8::updateInput(int buttonPressed) {
	key[buttonPressed] = true;
}

void Chip8::setCPURate(int newRate) {
	cpuRate = newRate;
}

int Chip8::getCPURate()
{
	return cpuRate;
}
