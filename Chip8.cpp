#include "Chip8.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>

const uint8_t FONTSET[80] = {
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

Chip8::Chip8():
    memory{},
    V{},
    I(0),
    PC(0x200),
    stack{},
    sp(0),
    delay_timer(0),
    sound_timer(0),
    display{},
    keypad{}
{
    for (int i = 0 ; i < 80 ; i++) {
        memory[i] = FONTSET[i];
    }
}

void Chip8::loadROM(const std::string& filename) {
    reset();

    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cout << "Could not open file " << filename << std::endl;
        return;
    }

    std::streampos size = file.tellg();

    if (size > 3584) {
        std::cout << "Error: ROM is too big for CHIP-8 memory (" << size << " bytes)." << std::endl;
        file.close();
        return;
    }

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(&memory[0x200]), size);
    file.close();

    std::cout << "Successfully loaded " << size << " bytes into memory." << std::endl;
}

void Chip8::ROMswitchWindow(sf::RenderWindow& window) {
    sf::Font font("../Font.ttf");
    sf::Text MenuText(font,loadedROMs);



    menuOpen = true;

    if (!ROMsLoaded) {

        for (const auto & entry : std::filesystem::directory_iterator(gamePath)) {
            gameList.push_back(entry.path().string());
        }


        MenuText.setStyle(sf::Text::Bold);
        MenuText.setFillColor(sf::Color::White);

        window.clear(sf::Color::Black);

        ROMsLoaded = true;
    }

    else {

        window.clear(sf::Color::Black);
        for (int i = 0 ; i < gameList.size() ; i++) {
            MenuText.setString(gameList[i]);
            if (i == selectedGame) {
                MenuText.setFillColor(sf::Color::Yellow);
            }
            else {
                MenuText.setFillColor(sf::Color::White);
            }


            MenuText.setPosition(sf::Vector2f(0, 100) + MenuText.getPosition());
            window.draw(MenuText);
        }

        window.display();


    }

}

uint16_t Chip8::FETCH() {
    uint8_t count = memory[PC];
    uint8_t countpp = memory[PC + 1];
    return (static_cast<uint16_t>(count) << 8) | countpp;
}

void Chip8::cycle() {
    uint16_t opcode = FETCH();
    PC += 2;

    if (opcode == 0x00E0) {

        for (int i = 0; i < 64 * 32; i++) {
            display[i] = 0;
        }
    }
    else if (opcode == 0x00EE) {
        sp--;
        PC = stack[sp];
    }
    else {
        switch (opcode & 0xF000) {
            case 0x1000: {
                PC = opcode & 0x0FFF;
                break;
            }
            case 0x2000: {
                stack[sp] = PC;
                sp++;
                PC = opcode & 0x0FFF;
                break;
            }
            case 0x3000: {
                uint8_t x = (opcode & 0x0F00) >> 8;
                uint8_t kk = opcode & 0x00FF;
                if (V[x] == kk) PC += 2;
                break;
            }
            case 0x4000: {
                uint8_t x = (opcode & 0x0F00) >> 8;
                uint8_t kk = opcode & 0x00FF;
                if (V[x] != kk) PC += 2;
                break;
            }
            case 0x5000: {
                uint8_t x = (opcode & 0x0F00) >> 8;
                uint8_t y = (opcode & 0x00F0) >> 4;
                if (V[x] == V[y]) PC += 2;
                break;
            }
            case 0x6000: {
                uint8_t x = (opcode & 0x0F00) >> 8;
                uint8_t kk = opcode & 0x00FF;
                V[x] = kk;
                break;
            }
            case 0x7000: {
                uint8_t x = (opcode & 0x0F00) >> 8;
                uint8_t kk = opcode & 0x00FF;
                V[x] = V[x] + kk;
                break;
            }
            case 0x8000: {
                uint8_t x = (opcode & 0x0F00) >> 8;
                uint8_t y = (opcode & 0x00F0) >> 4;

                switch (opcode & 0x000F) {
                    case 0x00: V[x] = V[y]; break;
                    case 0x01: V[x] |= V[y]; break;
                    case 0x02: V[x] &= V[y]; break;
                    case 0x03: V[x] ^= V[y]; break;
                    case 0x04: {
                        uint16_t result = V[x] + V[y];
                        V[0xF] = (result > 255) ? 1 : 0;
                        V[x] = static_cast<uint8_t>(result & 0xFF);
                        break;
                    }
                    case 0x05: {
                        uint8_t carry = (V[x] >= V[y]) ? 1 : 0;
                        V[x] = V[x] - V[y];
                        V[0xF] = carry;
                        break;
                    }
                    case 0x06: {
                        uint8_t lsb = V[x] & 0x1;
                        V[x] >>= 1;
                        V[0xF] = lsb;
                        break;
                    }
                    case 0x07: {
                        uint8_t carry = (V[y] >= V[x]) ? 1 : 0;
                        V[x] = V[y] - V[x];
                        V[0xF] = carry;
                        break;
                    }
                }
                break;
            }
            case 0x9000: {
                uint8_t x = (opcode & 0x0F00) >> 8;
                uint8_t y = (opcode & 0x00F0) >> 4;
                if (V[x] != V[y]) PC += 2;
                break;
            }
            case 0xA000: {
                I = opcode & 0x0FFF;
                break;
            }
            case 0xB000: {
                PC = (opcode & 0x0FFF) + V[0];
                break;
            }
            case 0xC000: {
                uint8_t x = (opcode & 0x0F00) >> 8;
                uint8_t kk = opcode & 0x00FF;
                uint8_t rng = static_cast<uint8_t>(std::rand() % 256);
                V[x] = rng & kk;
                break;
            }
            case 0xD000: {
                uint8_t x = (opcode & 0x0F00) >> 8;
                uint8_t y = (opcode & 0x00F0) >> 4;
                uint8_t height = opcode & 0x000F;

                uint8_t xPos = V[x] % 64;
                uint8_t yPos = V[y] % 32;

                V[0xF] = 0;

                for (int row = 0; row < height; row++) {
                    uint8_t spriteByte = memory[I + row];
                    if (yPos + row >= 32) break;

                    for (int col = 0; col < 8; col++) {
                        if (xPos + col >= 64) break;
                        uint8_t pixel = (spriteByte >> (7 - col)) & 0x1;

                        if (pixel == 1) {
                            int index = (xPos + col) + ((yPos + row) * 64);
                            if (display[index] == 1) {
                                V[0xF] = 1;
                            }
                            display[index] ^= 1;
                        }
                    }
                }
                break;
            }
            case 0xE000: {
                uint8_t x = (opcode & 0x0F00) >> 8;
                switch (opcode & 0x00FF) {
                    case 0x9E: {
                        if (isKeyPressed(V[x])) {
                            PC += 2;
                        }
                        break;
                    }
                    case 0xA1: {
                        if (!isKeyPressed(V[x])) {
                            PC += 2;
                        }
                        break;
                    }
                }
                break;
            }
            case 0xF000: {
                switch (opcode & 0x00FF) {
                    case 0x07: {
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        V[x] = delay_timer;
                        break;
                    }
                    case 0x15: {
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        delay_timer = V[x];
                        break;
                    }
                    case 0x18: {
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        sound_timer = V[x];
                        break;
                    }
                    case 0x0A: {
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        bool anyKeyPressed = false;

                        for (uint8_t i = 0; i < 16; i++) {
                            if (isKeyPressed(i)) {
                                V[x] = i;
                                anyKeyPressed = true;
                                break;
                            }
                        }

                        if (!anyKeyPressed) {
                            PC -= 2;
                        }
                        break;
                    }
                    case 0x1E: {
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        I = I + V[x];
                        break;
                    }
                    case 0x29: {
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        I = V[x] * 5;
                        break;
                    }
                    case 0x33: {
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        memory[I] = V[x] / 100;
                        memory[I + 1] = (V[x] / 10) % 10;
                        memory[I + 2] = V[x] % 10;
                        break;
                    }
                    case 0x55: {
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        for (int j = 0 ; j <= x ; j++) {
                            memory[I + j] = V[j];
                        }
                        break;
                    }
                    case 0x65: {
                        uint8_t x = (opcode & 0x0F00) >> 8;
                        for (int j = 0 ; j <= x ; j++) {
                            V[j] = memory[I + j];
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
}

void Chip8::updateTimers() {
    if (delay_timer > 0) {
        delay_timer--;
    }
    if (sound_timer > 0) {
        sound_timer--;
    }
}

void Chip8::reset() {

    PC = 0x200;
    I = 0;
    delay_timer = 0;
    sound_timer = 0;
    menuOpen = false;

    for (int i = 0 ; i < 4096 ; i++) {memory[i] = 0;}
    for (int i = 0 ; i < 16 ; i++) {V[i] = 0;}
    for (int i = 0 ; i < 16 ; i++) {stack[i] = 0;}
    for (int i = 0 ; i < 64 * 32 ; i++) {display[i] = 0;}
    for (int i = 0 ; i < 16 ; i++) {keypad[i] = 0;}

    for (int i = 0 ; i < 80 ; i++) {
        memory[i] = FONTSET[i];
    }


}