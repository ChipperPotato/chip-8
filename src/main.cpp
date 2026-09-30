#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <cstdint>
#include <iostream>
#include "../include/chip8.h"

struct Display {
	SDL_Window* window {nullptr};
	SDL_Renderer* renderer {nullptr};
	SDL_Texture* texture {nullptr};
};

constexpr int SCALE = 10;
constexpr int CYCLES = 10; // Should be about 600 IPS according to reddit

// Starts SDL
bool init(Display& display, uint8_t width, uint8_t height);

// Free media and shutdown SDL
void cleanup(Display& display);



int main(int argc, char* argv[]) {

	// Error checking arguments
	if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <rom>" << std::endl;
		return 1;
	}

	// Creates a chip8 object loads the ROM (first cli arg)
	Chip8 chip8;
	if (!chip8.loadROM(argv[1])) {
		std::cerr << "Failed to load ROM: " << argv[1] << std::endl;
	}

	// Call the constructor for the display stuff
	Display display;

	// Initialize the window using SDL
	if (init(display, chip8.width, chip8.height) == false) {
		SDL_Log("Unable to initialize program!\n");
		return 1;
	}

	bool quit{false};

	SDL_Event e;
	SDL_zero(e);

	while (quit == false) {
		Uint64 start = SDL_GetTicks();

		while (SDL_PollEvent(&e) == true) {
			if (e.type == SDL_EVENT_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_KEY_UP) {
			uint8_t state = (e.type == SDL_EVENT_KEY_DOWN) ? 1 : 0;

			switch (e.key.key) {
				case SDLK_ESCAPE: if (state) quit = true; break;
				case SDLK_X: chip8.keypad[0x0] = state; break;
				case SDLK_1: chip8.keypad[0x1] = state; break;
				case SDLK_2: chip8.keypad[0x2] = state; break;
				case SDLK_3: chip8.keypad[0x3] = state; break;
				case SDLK_Q: chip8.keypad[0x4] = state; break;
				case SDLK_W: chip8.keypad[0x5] = state; break;
				case SDLK_E: chip8.keypad[0x6] = state; break;
				case SDLK_A: chip8.keypad[0x7] = state; break;
				case SDLK_S: chip8.keypad[0x8] = state; break;
				case SDLK_D: chip8.keypad[0x9] = state; break;
				case SDLK_Z: chip8.keypad[0xA] = state; break;
				case SDLK_C: chip8.keypad[0xB] = state; break;
				case SDLK_4: chip8.keypad[0xC] = state; break;
				case SDLK_R: chip8.keypad[0xD] = state; break;
				case SDLK_F: chip8.keypad[0xE] = state; break;
				case SDLK_V: chip8.keypad[0xF] = state; break;
			}
		}
		}

		// Draw the screen
		if (chip8.drawFlag) {
			SDL_UpdateTexture(display.texture, nullptr, chip8.gfx,
			                  chip8.width * sizeof(uint32_t));
			SDL_RenderClear(display.renderer);
			SDL_RenderTexture(display.renderer, display.texture, nullptr, nullptr);
			SDL_RenderPresent(display.renderer);
			chip8.drawFlag = false;
		}

		// Run the CPU cycles
		for (int n = 0; n < CYCLES; n++) {
			chip8.cycle();
		}

		// Update the timer every frame
	  chip8.updateTimers();

		// Every frame should be 1 second / 60 = 16 ms
		Uint64 elapsed = SDL_GetTicks() - start;
		if (elapsed < 16) {
			SDL_Delay(16 - elapsed);
	  }
	}

	cleanup(display);
	return 0;

}

/* Function Implementations */
bool init(Display& display, uint8_t width, uint8_t height) {
  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) == false )
  {
    SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError() );
    return false;
  }

  // Create the window
	display.window = SDL_CreateWindow("Chippy-8 Emulator",
		                                width * SCALE, height * SCALE,
		  	                            SDL_WINDOW_RESIZABLE);
	if(display.window == nullptr) {
    SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
    return false;
	}

	// Create the renderer
	display.renderer = SDL_CreateRenderer(display.window, nullptr);
	if (display.renderer == nullptr) {
		SDL_Log("Renderer could not be created! SDL error: %s\n", SDL_GetError());
    return false;
	}

	// Create the texture
	display.texture = SDL_CreateTexture(display.renderer, SDL_PIXELFORMAT_RGBA8888,
	                                    SDL_TEXTUREACCESS_STREAMING, width, height);
	if (display.texture == nullptr) {
		SDL_Log("Texture could not be created! SDL error: %s\n", SDL_GetError());
    return false;
	}

	SDL_SetTextureScaleMode(display.texture, SDL_SCALEMODE_NEAREST);
  return true;
}

void cleanup(Display& display) {

	SDL_DestroyTexture(display.texture);
	SDL_DestroyRenderer(display.renderer);
  SDL_DestroyWindow(display.window);

  //Quit SDL subsystems
  SDL_Quit();
}
