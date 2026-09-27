#include <iostream>
#include "../include/chip8.h"


int main(int argc, char* argv[]) {

	// Error checking arguments
	if (argc != 2) {
		std::cerr << "Exactly 1 ROM permitted/required." << std::endl;
		return 1;
	}

	// Creates a chip8 object loads the ROM (first cli arg)
	Chip8 chip8;
	if (!chip8.loadROM(argv[1])) {
		std::cerr << "Failed to load ROM: " << argv[1] << std::endl;
	}

	// Emulate a single cycle
	chip8.cycle();

	return 0;

}
