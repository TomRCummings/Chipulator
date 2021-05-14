#include "chip8.h"
#include <iostream>
#include <fstream>

chip8 theChip8;

void saveChip8State(chip8 machine);

int main() {
    theChip8.initialize();
    saveChip8State(theChip8);

    return 0;
}

void saveChip8State(chip8 machine) {
    std::ofstream file;
    file.open("chip8state.c8", std::ios::binary);
    unsigned char* machineMemory = machine.getMemory();
    file.write((char *)&machineMemory[0], 4096);
    file.close();
}