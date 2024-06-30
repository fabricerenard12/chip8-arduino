// CHIP-8 Emulator Implementation
// Author: Fabrice Renard
// Date: 29/06/2024

#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define START_ADDRESS 0x200
#define FONTSET_START_ADDRESS 0x50
#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32

class Chip8 {
public:
    uint8_t registers[16];
    uint8_t memory[4096];
    uint16_t index;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t keypad[16];
    uint32_t video[64 * 32];
    uint16_t opcode;

    Chip8();
    void initialize();
    void loadFonts();
    bool loadROM(const char* path);
    uint8_t randomByte();
    void op00E0();
    void op00EE();
    void op1nnn();
    void op2nnn();
    void op3xkk();
    void op4xkk();
    void op5xy0();
    void op6xkk();
    void op7xkk();
    void op8xy0();
    void op8xy1();
    void op8xy2();
    void op8xy3();
    void op8xy4();
    void op8xy5();
    void op8xy6();
    void op8xy7();
    void op8xyE();
    void op9xy0();
    void opAnnn();
    void opBnnn();
    void opCxkk();
    void opDxyn();
    void opEx9E();
    void opExA1();
    void opFx07();
    void opFx0A();
    void opFx15();
    void opFx18();
    void opFx1E();
    void opFx29();
    void opFx33();
    void opFx55();
    void opFx65();
    void cycle();
};

#endif // CHIP8_HPP
