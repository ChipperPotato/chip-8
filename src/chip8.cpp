#include "../include/chip8.h"
#include <fstream>
#include <filesystem>

#include <iostream>
#include <iomanip>

// Most Chip-8 programs start at 0x200
const unsigned int START_ADDR = 0x200;

// Default constructor

Chip8::Chip8(): index(0), pc(0x200), sp(0),
                delayTimer(0), soundTimer(0) {

  // Load the fontset into memory
  for (int i = 0; i < 80; i++) {
  	memory[i] = fontset[i];
  }

}

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
	// FETCH 16-bit opcodes, remember, our memory is in bytes
	uint16_t opcode = (memory[pc] << 8 | memory[pc + 1]);

	// Update program counter
	pc += 2;

	// DECODE the opcode
	switch (opcode & 0xF000) {

		// Opcodes with most-significant-nibble of 0x0
    case 0x0000:
      switch (opcode & 0xFFF0) {
        case 0x00C0:
          OP_00Cn(opcode);
          break;
        default:
          switch (opcode) {
		        case 0x00E0:
		          OP_00E0(opcode);
		          break;
	          case 0x00EE:
	            OP_00EE(opcode);
	            break;
	          case 0x00FB:
	            OP_00FB(opcode);
	            break;
	          case 0x00FC:
	            OP_00FC(opcode);
	            break;
	          case 0x00FD:
	            OP_00FD(opcode);
	            break;
	          case 0x00FE:
	            OP_00FE(opcode);
	            break;
	          case 0x00FF:
	            OP_00FF(opcode);
	            break;
	          default:
	            OP_0nnn(opcode);
	            break;
	        }
	      	break;
			}
			break;

		// Opcodes with most-significant-nibble of 0x1 - 0x7
		case 0x1000:
			OP_1nnn(opcode);
			break;
		case 0x2000:
			OP_2nnn(opcode);
			break;
		case 0x3000:
			OP_3xkk(opcode);
			break;
		case 0x4000:
			OP_4xkk(opcode);
			break;
		case 0x5000:
			OP_5xy0(opcode);
			break;
		case 0x6000:
			OP_6xkk(opcode);
			break;
		case 0x7000:
			OP_7xkk(opcode);
			break;

		// Opcodes with most-significant-nibble of 0x8
		case 0x8000:
			switch (opcode & 0x000F) {
				case 0x0000:
					OP_8xy0(opcode);
					break;
				case 0x0001:
					OP_8xy1(opcode);
					break;
				case 0x0002:
					OP_8xy2(opcode);
					break;
				case 0x0003:
					OP_8xy3(opcode);
					break;
				case 0x0004:
					OP_8xy4(opcode);
					break;
				case 0x0005:
					OP_8xy5(opcode);
					break;
				case 0x0006:
					OP_8xy6(opcode);
					break;
				case 0x0007:
					OP_8xy7(opcode);
					break;
				case 0x000E:
					OP_8xyE(opcode);
					break;
				default:
					std::cout << "Unknown opcode: " << opcode << std::endl;
					break;
			}
			break;

		// Opcodes with most-significant-nibble of 0x9 - 0xC
		case 0x9000:
			OP_9xy0(opcode);
			break;
		case 0xA000:
			OP_Annn(opcode);
			break;
		case 0xB000:
			OP_Bnnn(opcode);
			break;
		case 0xC000:
			OP_Cxkk(opcode);
			break;

		// Opcodes with most-significant-nibble of 0xD
		case 0xD000:
			switch (opcode & 0x000F) {
				case 0x0000:
					OP_Dxy0(opcode);
					break;
				default:
					OP_Dxyn(opcode);
					break;
			}
			break;

		// Opcodes with most-significant-nibble of 0xE
		case 0xE000:
			switch (opcode & 0xF0FF) {
				case 0xE09E:
					OP_Ex9E(opcode);
					break;
				case 0xE0A1:
					OP_ExA1(opcode);
					break;
				default:
					std::cout << "Unknown opcode: " << opcode << std::endl;
					break;
			}
			break;

		// Opcodes with most-significant-nibble of 0xF
		case 0xF000:
			switch (opcode & 0xF0FF) {
				case 0xF007:
					OP_Fx07(opcode);
					break;
				case 0xF00A:
					OP_Fx0A(opcode);
					break;
				case 0xF015:
					OP_Fx15(opcode);
					break;
				case 0xF018:
					OP_Fx18(opcode);
					break;
				case 0xF01E:
					OP_Fx1E(opcode);
					break;
				case 0xF029:
					OP_Fx29(opcode);
					break;
				case 0xF033:
					OP_Fx33(opcode);
					break;
				case 0xF055:
					OP_Fx55(opcode);
					break;
				case 0xF065:
					OP_Fx65(opcode);
					break;
				case 0xF030:
					OP_Fx30(opcode);
					break;
				case 0xF075:
					OP_Fx75(opcode);
					break;
				case 0xF085:
					OP_Fx85(opcode);
					break;
				default:
					std::cout << "Unknown opcode: " << opcode << std::endl;
					break;
			}
			break;
		default:
			std::cout << "Unknown opcode: " << opcode << std::endl;
			break;
	}

	// DEBUG: Print out the current OP Code
	std::cout << "PC: 0x"
	          << std::hex               // Print in hexadecimal
	          << std::setfill('0')
	          << std::setw(3)           // PC represented in 3 hex digits
	          << this->pc
	          << " | Opcode: 0x"
	          << std::setw(4)           // Opcodes represented in 4 hex digits
	          << opcode
						<< std::dec;              // Back to decimal
}

// EXECUTE opcodes

// CLS
void OP_00E0(const uint16_t opcode) {

}

// RET
void OP_00EE(const uint16_t opcode);

// SYS add
void OP_0nnn(const uint16_t opcode);
// JP addr
void OP_1nnn(const uint16_t opcode);

// CALL addr
void OP_2nnn(const uint16_t opcode);

// SE Vx, byte
void OP_3xkk(const uint16_t opcode);

// SNE Vx, byte
void OP_4xkk(const uint16_t opcode);

// SE Vx, Vy
void OP_5xy0(const uint16_t opcode);

// LD Vx, byte
void OP_6xkk(const uint16_t opcode);

// ADD Vx, byte
void OP_7xkk(const uint16_t opcode);

// LD Vx, Vy
void OP_8xy0(const uint16_t opcode);

// OR Vx, Vy
void OP_8xy1(const uint16_t opcode);

// AND Vx, Vy
void OP_8xy2(const uint16_t opcode);

// XOR Vx, Vy
void OP_8xy3(const uint16_t opcode);

// ADD Vx, Vy
void OP_8xy4(const uint16_t opcode);

// SUB Vx, Vy
void OP_8xy5(const uint16_t opcode);

// SHR Vx {, Vy}
void OP_8xy6(const uint16_t opcode);

// SUBN Vx, Vy
void OP_8xy7(const uint16_t opcode);

// SHL Vx {, Vy}
void OP_8xyE(const uint16_t opcode);

// SNE Vx, Vy
void OP_9xy0(const uint16_t opcode);

// LD I, addr
void OP_Annn(const uint16_t opcode);

// JP V0, addr
void OP_Bnnn(const uint16_t opcode);

// RND Vx, byte
void OP_Cxkk(const uint16_t opcode);

// DRW Vx, Vy, nibble
void OP_Dxyn(const uint16_t opcode);

// SKP Vx
void OP_Ex9E(const uint16_t opcode);

// SKNP Vx
void OP_ExA1(const uint16_t opcode);

// LD Vx, DT
void OP_Fx07(const uint16_t opcode);

// LD Vx, K
void OP_Fx0A(const uint16_t opcode);

// LD DT, Vx
void OP_Fx15(const uint16_t opcode);

// LD ST, Vx
void OP_Fx18(const uint16_t opcode);

// ADD I, Vx
void OP_Fx1E(const uint16_t opcode);

// LD F, Vx
void OP_Fx29(const uint16_t opcode);

// LD B, Vx
void OP_Fx33(const uint16_t opcode);

// LD [I], Vx
void OP_Fx55(const uint16_t opcode);

// LD Vx, [I]
void OP_Fx65(const uint16_t opcode);
