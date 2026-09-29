#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <string>

class Chip8 {
private:
	bool drawFlag = false;   // Flag for keeping track of drawing screen
	uint8_t height, width;   // Keeps track of graphics resolution

	uint8_t memory[4096]{};  // 4 KiB of memory
	uint8_t registers[16]{}; // 16, 8-bit registers

	uint16_t index{};        // 16-bit index register
	uint16_t pc{};           // 16-bit program counter

	uint16_t stack[16]{};    // 16-level stack
	uint8_t sp{};            // 8-bit stack pointer

	uint32_t* gfx;
	uint8_t delayTimer{};    // 8-bit delay timer
	uint8_t soundTimer{};    // 8-bit sound timer

	// Keypad

	uint8_t keypad[16]{};

	// Font Set

	const uint8_t fontset[80] = {
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

	// OP codes, Cowgod's Chip-8 reference

	// Standard Chip-8
	void OP_00E0(const uint16_t opcode); // CLS
	void OP_00EE(const uint16_t opcode); // RET
	void OP_0nnn(const uint16_t opcode); // SYS addr
	void OP_1nnn(const uint16_t opcode); // JP addr
  void OP_2nnn(const uint16_t opcode); // CALL addr
  void OP_3xkk(const uint16_t opcode); // SE Vx, byte
  void OP_4xkk(const uint16_t opcode); // SNE Vx, byte
  void OP_5xy0(const uint16_t opcode); // SE Vx, Vy
  void OP_6xkk(const uint16_t opcode); // LD Vx, byte
  void OP_7xkk(const uint16_t opcode); // ADD Vx, byte
  void OP_8xy0(const uint16_t opcode); // LD Vx, Vy
  void OP_8xy1(const uint16_t opcode); // OR Vx, Vy
  void OP_8xy2(const uint16_t opcode); // AND Vx, Vy
  void OP_8xy3(const uint16_t opcode); // XOR Vx, Vy
  void OP_8xy4(const uint16_t opcode); // ADD Vx, Vy
  void OP_8xy5(const uint16_t opcode); // SUB Vx, Vy
  void OP_8xy6(const uint16_t opcode); // SHR Vx {, Vy}
  void OP_8xy7(const uint16_t opcode); // SUBN Vx, Vy
  void OP_8xyE(const uint16_t opcode); // SHL Vx {, Vy}
  void OP_9xy0(const uint16_t opcode); // SNE Vx, Vy
  void OP_Annn(const uint16_t opcode); // LD I, addr
  void OP_Bnnn(const uint16_t opcode); // JP V0, addr
  void OP_Cxkk(const uint16_t opcode); // RND Vx, byte
  void OP_Dxyn(const uint16_t opcode); // DRW Vx, Vy, nibble
  void OP_Ex9E(const uint16_t opcode); // SKP Vx
  void OP_ExA1(const uint16_t opcode); // SKNP Vx
  void OP_Fx07(const uint16_t opcode); // LD Vx, DT
  void OP_Fx0A(const uint16_t opcode); // LD Vx, K
  void OP_Fx15(const uint16_t opcode); // LD DT, Vx
  void OP_Fx18(const uint16_t opcode); // LD ST, Vx
  void OP_Fx1E(const uint16_t opcode); // ADD I, Vx
  void OP_Fx29(const uint16_t opcode); // LD F, Vx
  void OP_Fx33(const uint16_t opcode); // LD B, Vx
  void OP_Fx55(const uint16_t opcode); // LD [I], Vx
  void OP_Fx65(const uint16_t opcode); // LD Vx, [I]

  // Super Chip-48
  void OP_00Cn(const uint16_t opcode); // SCD nibble
  void OP_00FB(const uint16_t opcode); // SCR
  void OP_00FC(const uint16_t opcode); // SCL
  void OP_00FD(const uint16_t opcode); // EXIT
  void OP_00FE(const uint16_t opcode); // LOW
  void OP_00FF(const uint16_t opcode); // HIGH
  void OP_Dxy0(const uint16_t opcode); // DRW Vx, Vy, 0
  void OP_Fx30(const uint16_t opcode); // LD HF, Vx
  void OP_Fx75(const uint16_t opcode); // LD R, Vx
  void OP_Fx85(const uint16_t opcode); // LD Vx, R

	// Output and Input are not included here

public:
	Chip8();                             // Constructor for Chip-8
	Chip8(const std::string super);      // Constructor for Super Chip-8

	bool loadROM(const std::string& filename);
	void cycle();
	void updateTimers();

	~Chip8();
};

#endif
