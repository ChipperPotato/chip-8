#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <string>

class Chip8 {
private:
	uint8_t memory[4096]{};  // 4 KiB of memory
	uint8_t registers[16]{}; // 16, 8-bit registers

	uint16_t index{};        // 16-bit index register
	uint16_t pc{};           // 16-bit program counter

	uint16_t stack[16]{};    // 16-level stack
	uint8_t sp{};            // 8-bit stack pointer

	uint8_t delayTimer;      // 8-bit delay timer

	// Output and Input are not included here

public:
	Chip8();               // Constructor

	bool loadROM(const std::string& filename);
	void cycle();
	void updateTimers();


};

#endif
