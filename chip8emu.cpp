#include "chip8.h"
#include <iostream>
#include <fstream>
#include <string>

chip8 theChip8;

struct HexCharStruct
{
  unsigned char c;
  HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
  return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c)
{
  return HexCharStruct(_c);
}

void saveChip8State(chip8 machine);
void readFromRom(std::string filename, unsigned char* dataBuffer, size_t fileSize);

int main() {
    theChip8.initialize();

    std::string romFilePath = "random_number_test.ch8";
    //Max number of bytes in a Chip-8 ROM
    const size_t fileSize = 3586;
    //Create program buffer
    unsigned char dataBuffer[fileSize] = { 0 };
    readFromRom(romFilePath, dataBuffer, fileSize);
    //for (int i = 0; i < 3586; i++) {
    //    std::cout << hex(dataBuffer[i]) << " ";
    //    if (i % 100 == 0) {
    //        std::cout << std::endl;
    //    }
    //}
    theChip8.loadROM(dataBuffer);
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

void readFromRom(std::string filename, unsigned char* dataBuffer, size_t fileSize) {
    //Open file stream to specified file
    std::ifstream inf(filename.c_str());

    if (inf) {
        //Read in bytes from file to buffer
        inf.read((char*)&dataBuffer[0], fileSize);
    }
}