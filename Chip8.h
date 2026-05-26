#ifndef UNTITLED_CHIP8_H
#define UNTITLED_CHIP8_H
#endif //UNTITLED_CHIP8_H

#include <cstdint>
class Chip8 {
    public:
        Chip8();

    private:
    uint8_t memory[4096]; //RAM
    uint8_t V[16]; //V0-VF Reg
    uint8_t I; //Index Reg

    uint16_t PC; //Program Counter
    uint16_t stack[16]; //Stack

    uint8_t sp; //Stack Pointer
    uint8_t delay_timer; //Countdown for game delay
    uint8_t sound_timer; //Countdown for sound


};