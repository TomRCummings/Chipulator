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
    //Key state array (chip-8 used hexadecimal keypad)
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
};

void chip8::initialize() {
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