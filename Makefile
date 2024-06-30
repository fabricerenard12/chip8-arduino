# Makefile for CHIP-8 Emulator
# Author: Fabrice Renard
# Date: 28/06/2024

MCU = atmega328p
PROJECTNAME = chip8-arduino
SRC_DIR = src
INCLUDE_DIR = include
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(SRC_DIR)/%.o)

CXX = avr-g++
CXXFLAGS = -Os -DF_CPU=16000000UL -mmcu=$(MCU) -I$(INCLUDE_DIR)
LDFLAGS = -mmcu=$(MCU)

AVRDUDE = avrdude
AVRDUDE_PROGRAMMER = arduino
AVRDUDE_PORT = /dev/ttyACM0

INSTALL_CMD = $(AVRDUDE) -p $(MCU) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PORT) -U flash:w:$(PROJECTNAME).hex:i

.PHONY: all install clean

all: $(PROJECTNAME).hex

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(PROJECTNAME).elf: $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $@ $^

$(PROJECTNAME).hex: $(PROJECTNAME).elf
	avr-objcopy -O ihex -R .eeprom $< $@

install: $(PROJECTNAME).hex
	$(INSTALL_CMD)

clean:
	rm -f $(SRC_DIR)/*.o $(PROJECTNAME).elf $(PROJECTNAME).hex
