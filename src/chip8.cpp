#include "../include/chip8.h"
#include <fstream>
#include <filesystem>

#include <iostream>
#include <iomanip>

// Most Chip-8 programs start at 0x200
const unsigned int START_ADDR = 0x200;

// Load ROMs with their filename
bool Chip8::loadROM(const std::string& filename) {

	// Open as a binary file.
	std::ifstream file(filename, std::ios::binary);

	// Return false if the file cannot be opened
	if (!file.is_open()) {
		return false;
	}

	// Determines the size of the ROM so we know how much memory to allocate
	std::uintmax_t size = std::filesystem::file_size(filename);

	// False if the file is too large
	if (size > (4096 - START_ADDR)) {
		return false;
	}

	// Allocate memory for a temporary buffer to read in
	char* buffer = new char[size];

	// Read the file into the buffer and into memory
	file.read(buffer, size);

	for (std::uintmax_t i = 0; i < size; i++) {
		memory[START_ADDR + i] = buffer[i];
	}

	delete[] buffer;
	return true;
}

// Fetch/decode/execute loop
void Chip8::cycle() {
	// Fetch 16-bit opcodes, remember, our memory is in bytes
	uint16_t opcode = (memory[pc] << 8 | memory[pc + 1]);

	// DEBUG: Print out the current OP Code
	std::cout << "PC: 0x"
	          << std::hex               // Print in hexadecimal
	          << std::setw(3)           // PC represented in 3 hex digits
	          << this->pc
	          << " | Opcode: 0x"
	          << std::setw(4)           // Opcodes represented in 4 hex digits
	          << opcode
						<< std::dec << std::endl; // Back to decimal
}
