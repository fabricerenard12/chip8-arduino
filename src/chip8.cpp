// CHIP-8 Emulator Implementation
// Author: Fabrice Renard
// Date: 29/06/2024

#include "chip8.hpp"

Chip8::Chip8() : pc(START_ADDRESS), index(0), sp(0), delayTimer(0), soundTimer(0), opcode(0) {
    initialize();
}

void Chip8::initialize() {
    pc = START_ADDRESS;
    loadFonts();
    memset(memory, 0, sizeof(memory));
    memset(registers, 0, sizeof(registers));
    memset(stack, 0, sizeof(stack));
    memset(keypad, 0, sizeof(keypad));
    memset(video, 0, sizeof(video));
}

void Chip8::loadFonts() {
    uint8_t fonts[80] = {
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

    for (int i = 0; i < sizeof(fonts); i++) {
        memory[FONTSET_START_ADDRESS + i] = fonts[i];
    }
}

bool Chip8::loadROM(const char* path) {
    // TODO
}

uint8_t Chip8::randomByte() {
    return rand() % 256;
}

void Chip8::op00E0() {
    memset(video, 0, sizeof(video));
}

void Chip8::op00EE() {
    --sp;
    pc = stack[sp];
}

void Chip8::op1nnn() {
    pc = opcode & 0x0FFF;
}

void Chip8::op2nnn() {
    stack[sp] = pc;
    ++sp;
    pc = opcode & 0x0FFF;
}

void Chip8::op3xkk() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    if (registers[x] == byte) {
        pc += 2;
    }
}

void Chip8::op4xkk() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    if (registers[x] != byte) {
        pc += 2;
    }
}

void Chip8::op5xy0() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (registers[x] == registers[y]) {
        pc += 2;
    }
}

void Chip8::op6xkk() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    registers[x] = byte;
}

void Chip8::op7xkk() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    registers[x] += byte;
}

void Chip8::op8xy0() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    registers[x] = registers[y];
}

void Chip8::op8xy1() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    registers[x] |= registers[y];
}

void Chip8::op8xy2() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    registers[x] &= registers[y];
}

void Chip8::op8xy3() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    registers[x] ^= registers[y];
}

void Chip8::op8xy4() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint16_t sum = registers[x] + registers[y];
    registers[0xF] = sum > 255 ? 1 : 0;
    registers[x] = sum & 0xFF;
}

void Chip8::op8xy5() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    registers[0xF] = registers[x] > registers[y] ? 1 : 0;
    registers[x] -= registers[y];
}

void Chip8::op8xy6() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    registers[0xF] = registers[x] & 1;
    registers[x] >>= 1;
}

void Chip8::op8xy7() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    registers[0xF] = registers[y] > registers[x] ? 1 : 0;
    registers[x] = registers[y] - registers[x];
}

void Chip8::op8xyE() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    registers[0xF] = registers[x] >> 7;
    registers[x] <<= 1;
}

void Chip8::op9xy0() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (registers[x] != registers[y]) {
        pc += 2;
    }
}

void Chip8::opAnnn() {
    index = opcode & 0x0FFF;
}

void Chip8::opBnnn() {
    pc = (opcode & 0x0FFF) + registers[0];
}

void Chip8::opCxkk() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;
    registers[x] = randomByte() & byte;
}

void Chip8::opDxyn() {
    uint8_t x = registers[(opcode & 0x0F00) >> 8];
    uint8_t y = registers[(opcode & 0x00F0) >> 4];
    uint8_t height = opcode & 0x000F;
    registers[0xF] = 0;

    for (unsigned int row = 0; row < height; ++row) {
        uint8_t spriteByte = memory[index + row];
        for (unsigned int col = 0; col < 8; ++col) {
            uint8_t spritePixel = spriteByte & (0x80 >> col);
            uint32_t* screenPixel = &video[(y + row) * 64 + (x + col)];

            if (spritePixel) {
                if (*screenPixel == 0xFFFFFFFF) {
                    registers[0xF] = 1;
                }
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void Chip8::opEx9E() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    if (keypad[registers[x]]) {
        pc += 2;
    }
}

void Chip8::opExA1() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    if (!keypad[registers[x]]) {
        pc += 2;
    }
}

void Chip8::opFx07() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    registers[x] = delayTimer;
}

void Chip8::opFx0A() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    bool keyPress = false;
    for (int i = 0; i < 16; ++i) {
        if (keypad[i]) {
            registers[x] = i;
            keyPress = true;
            break;
        }
    }

    if (!keyPress) {
        pc -= 2;
    }
}

void Chip8::opFx15() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    delayTimer = registers[x];
}

void Chip8::opFx18() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    soundTimer = registers[x];
}

void Chip8::opFx1E() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    index += registers[x];
}

void Chip8::opFx29() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    index = FONTSET_START_ADDRESS + (registers[x] * 5);
}

void Chip8::opFx33() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    memory[index + 0] = registers[x] / 100;
    memory[index + 1] = (registers[x] / 10) % 10;
    memory[index + 2] = registers[x] % 10;
}

void Chip8::opFx55() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    for (uint8_t i = 0; i <= x; ++i) {
        memory[index + i] = registers[i];
    }
}

void Chip8::opFx65() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    for (uint8_t i = 0; i <= x; ++i) {
        registers[i] = memory[index + i];
    }
}

void Chip8::cycle() {
    opcode = (memory[pc] << 8u) | memory[pc + 1];
    pc += 2;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0: op00E0(); break;
                case 0x00EE: op00EE(); break;
                default: break;
            }
            break;
        case 0x1000: op1nnn(); break;
        case 0x2000: op2nnn(); break;
        case 0x3000: op3xkk(); break;
        case 0x4000: op4xkk(); break;
        case 0x5000: op5xy0(); break;
        case 0x6000: op6xkk(); break;
        case 0x7000: op7xkk(); break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000: op8xy0(); break;
                case 0x0001: op8xy1(); break;
                case 0x0002: op8xy2(); break;
                case 0x0003: op8xy3(); break;
                case 0x0004: op8xy4(); break;
                case 0x0005: op8xy5(); break;
                case 0x0006: op8xy6(); break;
                case 0x0007: op8xy7(); break;
                case 0x000E: op8xyE(); break;
                default: break;
            }
            break;
        case 0x9000: op9xy0(); break;
        case 0xA000: opAnnn(); break;
        case 0xB000: opBnnn(); break;
        case 0xC000: opCxkk(); break;
        case 0xD000: opDxyn(); break;
        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x009E: opEx9E(); break;
                case 0x00A1: opExA1(); break;
                default: break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x0007: opFx07(); break;
                case 0x000A: opFx0A(); break;
                case 0x0015: opFx15(); break;
                case 0x0018: opFx18(); break;
                case 0x001E: opFx1E(); break;
                case 0x0029: opFx29(); break;
                case 0x0033: opFx33(); break;
                case 0x0055: opFx55(); break;
                case 0x0065: opFx65(); break;
                default: break;
            }
            break;
        default: break;
    }

    if (delayTimer > 0) {
        --delayTimer;
    }

    if (soundTimer > 0) {
        --soundTimer;
    }
}

