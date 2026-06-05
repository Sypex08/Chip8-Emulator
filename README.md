# CHIP-8 Emulator

A lightweight, hardware-level CHIP-8 emulator written in modern C++. This project utilizes SFML 3 for graphics rendering and keyboard input handling.

The main focus of this project was to gain a hands-on understanding of CPU architectures, the fetch-decode-execute cycle, low-level bit manipulation, and memory mapping.

## Features
* **Complete Instruction Set:** Full emulation of all standard CHIP-8 opcodes, including delay and sound timer logic.
* **Modern SFML 3 Backend:** Leverages the updated, type-safe SFML 3 event system (`std::optional` event handling) for smooth rendering and responsive inputs.
* **Classic Clock Timing:** The CPU cycle runs independently of the framerate at ~500 Hz, while the delay and sound timers decrement at the standard 60 Hz.
* **Robust Graphics Mapping:** Implementation of the original sprite rendering via XOR operations, complete with accurate collision detection (VF register).

<img width="1919" height="1079" alt="image" src="https://github.com/user-attachments/assets/640af5c8-5fdd-489b-b304-44225a839e87" />

## Keyboard Layout
The original 4x4 hex keypad of the CHIP-8 has been mapped to a modern QWERTY/QWERTZ layout:

```text
Original CHIP-8 Keypad:      PC Keyboard Mapping:
+-+-+-+-+                    +-+-+-+-+
|1|2|3|C|                    |1|2|3|4|
+-+-+-+-+                    +-+-+-+-+
|4|5|6|D|                    |Q|W|E|R|
+-+-+-+-+                    +-+-+-+-+
|7|8|9|E|                    |A|S|D|F|
+-+-+-+-+                    +-+-+-+-+
|A|0|B|F|                    |Z|X|C|V|  (Note: Use Y instead of Z for QWERTZ)
+-+-+-+-+                    +-+-+-+-+
```

## ROM Selection Menu Controls

While the emulator is running, you can access the built-in ROM selection menu to browse and load different games without restarting the application.

| Key | Action |
| :--- | :--- |
| **Escape (Esc)** | Open / Close the ROM selection menu |
| **Numpad 1 (NUM 1)** | Navigate UP through the game list |
| **Numpad 2 (NUM 2)** | Navigate DOWN through the game list |
| **Numpad 3 (NUM 3)** | Boot / Start the currently selected game |

*Note: When the menu is open, the standard CHIP-8 keypad inputs are paused until you close the menu or start a game.*

## Prerequisites & Dependencies
Compiler: C++17 or newer (tested with GCC / Clang on CLion)

Build System: CMake

Frameworks: SFML 3 (installed and managed via vcpkg)

## Installation & Compilation
Clone the repository:

```Bash
git clone [https://github.com/YOUR_USERNAME/chip8-emulator.git](https://github.com/YOUR_USERNAME/chip8-emulator.git)
cd chip8-emulator
```
Install dependencies (vcpkg):
Ensure SFML 3 is installed through your vcpkg setup:

```Bash
vcpkg install sfml
```
Build the project (CMake):

```Bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[PATH_TO_YOUR_VCPKG]/scripts/buildsystems/vcpkg.cmake
cmake --build .
```
## Project Structure
Chip8.h / Chip8.cpp – Core CPU logic, handles registers, 4KB RAM, and opcode execution.

main.cpp – Manages the SFML window, core application loop, display rendering, and CPU clock timing.

## Roadmap / Upcoming Features
[x] Integrate an audio library to make the sound timer audible.

[x] Add a native file-dialog picker for easily loading ROMs at runtime.

[x] Add scrolling in the ROM switch menu to make displaying as many ROMs as the user wants possible

Credits to https://github.com/kripod for releasing the ROMs!