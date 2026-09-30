## Chipper's Chippy Chip-8 Emulator

A CHIP-8 emulator written in C++17 with SDL3 for graphics and input. Capable of running Chip-8 games!

![Chippy-8 running a breakout clone.](docs/br8kout.png)

![Chippy-8 running a bullet hell game.](docs/dinorun.png)

All ROMs here are from johnearnest's [website](https://johnearnest.github.io/chip8Archive/?sort=platform). The two ROMs are Br8kout by SharpenedSpoon and Dino Run by AndrezInrc.

### Features

- Standard CHIP-8 instruction set (fetch / decode / execute loop)!
- 64x32 display, scaled up 10x with crisp nearest-neighbor pixels with SDL3!
- Resizable window!
- Monochrome display using a GBA color palette!
