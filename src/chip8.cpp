#include "../include/chip8.h"
#include <cstring>
#include <fstream>
#include <filesystem>

#include <iostream>
#include <iomanip>

// Most Chip-8 programs start at 0x200
const unsigned int START_ADDR = 0x200;

// Default constructor
Chip8::Chip8(): index(0), pc(0x200), sp(0),
                delayTimer(0), soundTimer(0) {

	height = 32;
	width = 64;

  // Gfx array for regular Chip-8
  gfx = new uint32_t[height * width]{};

	for (int i = 0; i < width * height; i++) {
		gfx[i] = 0x306230FF;
	}

  // Random number generation
	srand(time(nullptr));


  // Load the fontset into memory
  for (int i = 0; i < 80; i++) {
  	memory[0x50 + i] = fontset[i];
  }

}

// Super Chip-8 constructor
Chip8::Chip8(const std::string& super): index(0), pc(0x200), sp(0),
                                        delayTimer(0), soundTimer(0) {

	height = 64;
	width = 128;

  // Gfx array for Super Chip-8
  gfx = new uint32_t[height * width]{};

	for (int i = 0; i < width * height; i++) {
		gfx[i] = 0x306230FF;
	}

  // Random number generation
	srand(time(nullptr));

  // Load the fontset into memory
  for (int i = 0; i < 80; i++) {
  	memory[0x50 + i] = fontset[i];
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

	// DEBUG: Print out the current OP Code
	// std::cout << "PC: 0x"
	//           << std::hex               // Print in hexadecimal
	//           << std::setfill('0')
	//           << std::setw(3)           // PC represented in 3 hex digits
	//           << this->pc
	//           << " | Opcode: 0x"
	//           << std::setw(4)           // Opcodes represented in 4 hex digits
	//           << opcode
	// 					<< std::dec               // Back to decimal
	// 					<< std::endl;

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
					std::cout << std::hex << "Unknown opcode: "
					          << opcode  << std::dec << std::endl;
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
					std::cout << std::hex << "Unknown opcode: "
					          << opcode  << std::dec << std::endl;
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
					std::cout << std::hex << "Unknown opcode: "
					          << opcode  << std::dec << std::endl;
					break;
			}
			break;
		default:
			std::cout << std::hex << "Unknown opcode: "
			          << opcode  << std::dec << std::endl;
			break;
	}
}

// EXECUTE opcodes

// ----------- Standard Chip-8 -----------

// CLS
void Chip8::OP_00E0(const uint16_t opcode) {
	// Zero out the screen
	for (int i = 0; i < width * height; i++) {
		gfx[i] = 0x306230FF;
	}
	drawFlag = true;
}

// RET
void Chip8::OP_00EE(const uint16_t opcode) {
	--sp;
	pc = stack[sp];
}

// SYS addr
void Chip8::OP_0nnn(const uint16_t opcode) {
	// ignored instruction
}

// JP addr
void Chip8::OP_1nnn(const uint16_t opcode) {
	pc = (opcode & 0x0FFF);
}

// CALL addr
void Chip8::OP_2nnn(const uint16_t opcode) {
	stack[sp++] = pc;
	pc = opcode & 0x0FFF;
}

// SE Vx, byte
void Chip8::OP_3xkk(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	if (registers[x] == (opcode & 0x00FF)) {
		pc += 2;
	}
}

// SNE Vx, byte
void Chip8::OP_4xkk(const uint16_t opcode) {
	if (registers[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
		pc += 2;
	}
}

// SE Vx, Vy
void Chip8::OP_5xy0(const uint16_t opcode) {
	if (registers[(opcode & 0x0F00) >> 8] == registers[(opcode & 0x00F0) >> 4]) {
		pc += 2;
	}
}

// LD Vx, byte
void Chip8::OP_6xkk(const uint16_t opcode) {
	registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
}

// ADD Vx, byte
void Chip8::OP_7xkk(const uint16_t opcode) {
	registers[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
}

// LD Vx, Vy
void Chip8::OP_8xy0(const uint16_t opcode) {
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	registers[Vx] = registers[Vy];
}

// OR Vx, Vy
void Chip8::OP_8xy1(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	registers[x] = registers[x] | registers[y];

	// AND, OR, XOR reset vF
	// registers[0xF] = 0;
}

// AND Vx, Vy
void Chip8::OP_8xy2(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	registers[x] = registers[x] & registers[y];

	// AND, OR, XOR reset vF
	// registers[0xF] = 0;
}

// XOR Vx, Vy
void Chip8::OP_8xy3(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	registers[x] = registers[x] ^ registers[y];

	// AND, OR, XOR reset vF
	// registers[0xF] = 0;
}

// ADD Vx, Vy
void Chip8::OP_8xy4(const uint16_t opcode) {
	uint8_t Vx = registers[(opcode & 0x0F00) >> 8];
	uint8_t Vy = registers[(opcode & 0x00F0) >> 4];

	// uint16_t so we can detect overflows
	uint16_t sum = Vx + Vy;

	registers[(opcode & 0x0F00) >> 8] = sum & 0xFF;
	registers[0xF] = sum > 255 ? 1 : 0;
}

// SUB Vx, Vy
void Chip8::OP_8xy5(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	// This is needed to succeed in edge case where VF is an operand
	uint8_t flag = registers[x] > registers[y] ? 1 : 0;

	registers[x] -= registers[y];
	registers[0xF] = flag;
}

// SHR Vx {, Vy}
void Chip8::OP_8xy6(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t flag = registers[x] & 0x1;

	registers[x] = registers[x] >> 1;
	registers[0xF] = flag;
}

// SUBN Vx, Vy
void Chip8::OP_8xy7(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t y = (opcode & 0x00F0) >> 4;

	// This is needed to succeed in edge case where VF is an operand
	uint8_t flag = registers[y] > registers[x] ? 1 : 0;

	registers[x] = registers[y] - registers[x];
	registers[0xF] = flag;
}

// SHL Vx {, Vy}
void Chip8::OP_8xyE(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;

	// This is needed to succeed in edge case where VF is an operand
	uint8_t flag = (registers[x] & 0x80) >> 7;

	registers[x] = registers[x] << 1;
	registers[0xF] = flag;
}

// SNE Vx, Vy
void Chip8::OP_9xy0(const uint16_t opcode) {
	uint8_t Vx = registers[(opcode & 0x0F00) >> 8];
	uint8_t Vy = registers[(opcode & 0x00F0) >> 4];

	if (Vx != Vy) {
		pc += 2;
	}
}

// LD I, addr
void Chip8::OP_Annn(const uint16_t opcode) {
	index = (opcode & 0x0FFF);
}

// JP V0, addr
void Chip8::OP_Bnnn(const uint16_t opcode) {
	pc = registers[0x0] + (opcode & 0x0FFF);
}

// RND Vx, byte
void Chip8::OP_Cxkk(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	registers[x] = (rand() % 256) & (opcode & 0x00FF);
}

// DRW Vx, Vy, nibble
void Chip8::OP_Dxyn(const uint16_t opcode) {
    const uint8_t x = (opcode & 0x0F00) >> 8;
    const uint8_t y = (opcode & 0x00F0) >> 4;
    const uint8_t rows = opcode & 0x000F;

    // Wrap sprite position when outside the display coordinates
    const uint8_t xPos = registers[x] % width;
    const uint8_t yPos = registers[y] % height;

    // Clear the collision flag for now
    registers[0xF] = 0;

    for (int i = 0; i < rows; i++) {
        const uint8_t spriteByte = memory[index + i];
        for (int j = 0; j < 8; j++) {
            // Draw and test for collisions one bit at a time

            // This is if we want clipping
						if (!(spriteByte & (0x80 >> j))) continue;
            if (xPos + j >= width || yPos + i >= height) continue;

            uint32_t &pixel = gfx[(yPos + i) * width + (xPos + j)];

            // Set VF if there was a collision
            if (pixel == 0x9BBC0FFF) registers[0xF] = 1;

            // XOR sprite onto the screen
            pixel ^= (0x9BBC0FFF ^ 0x306230FF);


						// This is if we want wrapping
						// if (spriteByte & (0x80 >> j)) {
						//     uint8_t px = (xPos + j) % width;
						//     uint8_t py = (yPos + i) % height;
						//     uint32_t &pixel = gfx[py * width + px];

						//     // Set VF if there was a collision
						//     if (pixel == 0xFFFFFFFF) {
						//         registers[0xF] = 1;
						//     }

						//     // XOR sprite onto the screen
						//     pixel ^= 0xFFFFFFFF;
						// }
        }
    }
    drawFlag = true;
}

// SKP Vx
void Chip8::OP_Ex9E(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t key = registers[x];

	if (keypad[key]) {
		pc += 2;
	}
}

// SKNP Vx
void Chip8::OP_ExA1(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t key = registers[x];

	if (!keypad[key]) {
		pc += 2;
	}
}

// LD Vx, DT
void Chip8::OP_Fx07(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	registers[x] = delayTimer;
}

// LD Vx, K
void Chip8::OP_Fx0A(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;

	if (wait < 0) {
		// Checks for key presses
		for (uint8_t i = 0; i < 16; i++) {
			if (keypad[i]) {
				// store i and wait
				wait = i;
				break;
			}
		}
	} else if (!keypad[wait]) {
		registers[x] = wait;
		wait = -1;
		return;
	}

	// If we reached here, which means no keypress yet,
	// thus we repeat this instruction by decrementing PC
	pc -= 2;
}

// LD DT, Vx
void Chip8::OP_Fx15(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;

	delayTimer = registers[x];
}

// LD ST, Vx
void Chip8::OP_Fx18(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;

	soundTimer = registers[x];
}

// ADD I, Vx
void Chip8::OP_Fx1E(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;

	index += registers[x];
}

// LD F, Vx
void Chip8::OP_Fx29(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;
	uint8_t fontSprite = registers[x];

	index = 0x050 + (fontSprite * 5);
}

// LD B, Vx
void Chip8::OP_Fx33(const uint16_t opcode) {
	uint8_t Vx = registers[(opcode & 0x0F00) >> 8];

	// Ones digit at I + 2
	memory[index + 2] = Vx % 10;

	// Tens digit at I + 1
	memory[index + 1] = (Vx / 10) % 10;

	// Hundreds digit at I
	memory[index] = (Vx / 100) % 100;
}

// LD [I], Vx
void Chip8::OP_Fx55(const uint16_t opcode) {
	uint8_t x = (opcode & 0x0F00) >> 8;

	for (uint8_t i = 0; i <= x; i++) {
		memory[index + i] = registers[i];
	}
}

// LD Vx, [I]
void Chip8::OP_Fx65(const uint16_t opcode) {

	uint8_t x = (opcode & 0x0F00) >> 8;

	for (uint8_t i = 0; i <= x; i++) {
		registers[i] = memory[index + i];
	}
}

// ------------ Super Chip-48 ------------

// SCD nibble
void Chip8::OP_00Cn(const uint16_t opcode) {

}

// SCR
void Chip8::OP_00FB(const uint16_t opcode) {

}

// SCL
void Chip8::OP_00FC(const uint16_t opcode) {

}

// EXIT
void Chip8::OP_00FD(const uint16_t opcode) {

}

// LOW
void Chip8::OP_00FE(const uint16_t opcode) {

}

// HIGHT
void Chip8::OP_00FF(const uint16_t opcode) {

}

// DRW Vx, Vy, 0
void Chip8::OP_Dxy0(const uint16_t opcode) {

}

// LD HF, Vx
void Chip8::OP_Fx30(const uint16_t opcode) {

}

// LD R, Vx
void Chip8::OP_Fx75(const uint16_t opcode) {

}

// LD Vx, R
void Chip8::OP_Fx85(const uint16_t opcode) {

}

// Update Timers
void Chip8::updateTimers() {
	if (delayTimer > 0) {
		delayTimer--;
	}
	if (soundTimer > 0) {
		soundTimer--;
	}
}

// Destructor for the class
Chip8::~Chip8() {
	delete[] gfx;
}
