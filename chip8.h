#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class chip8 {
    //The current opcode to be executed
    unsigned short opcode;
    //4KB memory array
    unsigned char memory[4096];
    //16 8-bit registers
    unsigned char V[16];
    //16-bit register (stores memory addresses, so rightmost 12 bit are used)
    unsigned short I;
    //Screen array (64x32 pixels)
    bool screen[2048];
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
    void cycle();
    bool* getScreen();
    void updateInput(unsigned char buttonPressed);
};

void chip8::initialize() {
    srand (time(NULL));
    opcode = 0x0;
    I = 0x0;
    pc = 0x200;
    stackPointer = 0x0;
    delayTimer = 0x0;
    soundTimer= 0x0;
    for(int i = 0; i < 4096; i++) {
        if (i < 80) {
            memory[i] = chip8FontSet[i];
        } else {
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

void chip8::loadROM(unsigned char program[]) {
    for (int i = 512; i < 4096; i++) {
        memory[i] = program[i-512];
    }
}

unsigned char* chip8::getMemory() {
    return memory;
}

void chip8::cycle() {
    bool keyPressed = false;
    //Get the current opcode from memory using the program counter
    opcode = memory[pc] << 8 | memory[pc+1];
    //Decode and execute opcode in a switch statement
    switch(opcode & 0xF000) {
        case 0xA000: //0xAnnn: Set register I equal to nnn
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0xD000: //0xDxyn: Display n-byte sprite at (register x, register y), starting at memory location I
            for (int i = 0; i < (opcode & 0x000F); i++) {
                for (int j = 1; j < 8; j--) {
                    if ((memory[I+i] >> (8-j)) & 0x0001 != 0) {
                        if (screen[(((V[(opcode & 0x00F0)>>4]+i)*64)%2048)+((V[(opcode & 0x0F00)>>8]+(j-1))%64)]) {
                            V[0xF] = 0xFF;
                            screen[(((V[(opcode & 0x00F0)>>4]+i)*64)%2048)+((V[(opcode & 0x0F00)>>8]+(j-1))%64)] = false;
                        } else {
                            screen[(((V[(opcode & 0x00F0)>>4]+i)*64)%2048)+((V[(opcode & 0x0F00)>>8]+(j-1))%64)] = true;
                        }
                    }
                }
            }
            pc += 2;
            break;
        case 0xF000:
            switch(opcode & 0x00FF) {
                case 0x000A: //0xFx0A: Wait for key press, store key value in register x
                    for (int i = 0; i < 16; i++) {
                        if (key[i]) {
                            V[(opcode & 0x0F00) >> 8] = i;
                            keyPressed = true;
                        }
                    }
                    if (keyPressed) {
                        pc += 2;
                    }
                    break;
                case 0x0007: //0xFx07: Set register x to the delay timer value
                    V[(opcode & 0x0F00) >> 8] = delayTimer;
                    pc += 2;
                    break; 
                case 0x0029: //0xFx29: Value of I is set to the location of the sprite for the value of register x
                    I = V[(opcode & 0x0F00) >> 8] * 5;
                    pc += 2;
                    break;
                case 0x0033: //0xFx33: Store BCD representation of value at register x in memory location I, I+1, and I+2
                    memory[I] = V[(opcode & 0x0F00) >> 8]/100; //100's digit at memory address I
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8]/10)%10; //10's digit at memory address I+1
                    memory[I + 2] = (V[(opcode & 0x0F00) >> 8]%100)%10; //1's digit at memory address I+2
                    pc+=2;
                    break;
                case 0x0055: //0xFx55: Starting at memory address I, puts values from registers 0 to x into memory
                    for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
                        memory[I+i] = V[i];
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
        case 0x6000: //0x6xkk: Set register x equal to kk
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;
        case 0x2000: //0x2nnn: Go to subroutine at address nnn
            stack[stackPointer] = pc;
            stackPointer++;
            pc = opcode & 0x0FFF;
            break;
        case 0x7000: //0x7xkk: Add kk to the value in register x and put that into register x
            V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] + (opcode & 0x00FF);
            pc += 2;
            break;
        case 0xC000: //0xCxkk: Generate random byte, AND it with kk, and store in register x
            V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
            pc += 2;
            break;

        case 0x0000:
            switch(opcode & 0x00FF) {
                case 0x00E0: //0x00E0: Clear display
                    for (int i = 0; i < 2048; i++) {
                        screen[i] = false;
                    }
                    pc += 2;
                    break;
                
                case 0x00EE: //0x00EE: Return from subroutine
                    stackPointer--;
                    pc = stack[stackPointer];
                    break;

                case 0x0000: //0x0nnn: Deprecated (used to jump to nnn)
                    pc += 2;
                    break;
            }
            break;

        default: //Any unrecognizable opcode should throw an error and print the opcode
            printf("Error with opcode: 0x%x\n", opcode);
    }
}

bool* chip8::getScreen() {
    return screen;
}

void chip8::updateInput(unsigned char buttonPressed) {
    for (int i = 0; i < 16; i++) {
        key[i] = false;
    }
    if (buttonPressed != 0xFF) {
        key[buttonPressed] = true;
    }
}