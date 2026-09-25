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


	// OP codes, Cowgod's Chip-8 reference

	// Standard Chip-8
	void OP_00E0(); // CLS
	void OP_00EE(); // RET
	void OP_0nnn(); // SYS addr
	void OP_1nnn(); // JP addr
  void OP_2nnn(); // CALL addr
  void OP_3xkk(); // SE Vx, byte
  void OP_4xkk(); // SNE Vx, byte
  void OP_5xy0(); // SE Vx, Vy
  void OP_6xkk(); // LD Vx, byte
  void OP_7xkk(); // ADD Vx, byte
  void OP_8xy0(); // LD Vx, Vy
  void OP_8xy1(); // OR Vx, Vy
  void OP_8xy2(); // AND Vx, Vy
  void OP_8xy3(); // XOR Vx, Vy
  void OP_8xy4(); // ADD Vx, Vy
  void OP_8xy5(); // SUB Vx, Vy
  void OP_8xy6(); // SHR Vx {, Vy}
  void OP_8xy7(); // SUBN Vx, Vy
  void OP_8xyE(); // SHL Vx {, Vy}
  void OP_9xy0(); // SNE Vx, Vy
  void OP_Annn(); // LD I, addr
  void OP_Bnnn(); // JP V0, addr
  void OP_Cxkk(); // RND Vx, byte
  void OP_Dxyn(); // DRW Vx, Vy, nibble
  void OP_Ex9E(); // SKP Vx
  void OP_ExA1(); // SKNP Vx
  void OP_Fx07(); // LD Vx, DT
  void OP_Fx0A(); // LD Vx, K
  void OP_Fx15(); // LD DT, Vx
  void OP_Fx18(); // LD ST, Vx
  void OP_Fx1E(); // ADD I, Vx
  void OP_Fx29(); // LD F, Vx
  void OP_Fx33(); // LD B, Vx
  void OP_Fx55(); // LD [I], Vx
  void OP_Fx65(); // LD Vx, [I]

  // Super Chip-48
  void OP_00Cn(); // SCD nibble
  void OP_00FB(); // SCR
  void OP_00FC(); // SCL
  void OP_00FD(); // EXIT
  void OP_00FE(); // LOW
  void OP_00FF(); // HIGH
  void OP_Dxy0(); // DRW Vx, Vy, 0
  void OP_Fx30(); // LD HF, Vx
  void OP_Fx75(); // LD R, Vx
  void OP_Fx85(); // LD Vx, R

	// Output and Input are not included here

public:
	Chip8();               // Constructor

	bool loadROM(const std::string& filename);
	void cycle();
	void updateTimers();


};

#endif
