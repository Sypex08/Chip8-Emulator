#ifndef UNTITLED_CHIP8_H
#define UNTITLED_CHIP8_H

#include <string>
#include <cstdint>
#include <filesystem>
#include <vector>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Chip8 {
public:
    bool menuOpen = false;
    bool ROMsLoaded = false;
    Chip8();
    void loadROM(const std::string& filename);
    uint16_t FETCH();
    void cycle();
    int selectedGame = 0;
    void updateTimers();
    const uint8_t* getDisplay() const { return display; }
    void reset();
    uint8_t getSoundTimer() const { return sound_timer;}
    void ROMswitchWindow(sf::RenderWindow& window, const sf::Font& font);
    std::string gamePath;
    std::vector<std::string> gameList;
    int maxROMsOnScreen = 6;
    int ROMloadStartPosition = 0;



    void setKeyState(uint8_t key, bool isPressed) {
        if (key < 16) keypad[key] = isPressed ? 1 : 0;
    }

    
    bool isKeyPressed(uint8_t key) const {
        return (key < 16) ? (keypad[key] == 1) : false;
    }


private:
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;

    uint16_t PC;
    uint16_t stack[16];

    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;

    uint8_t display[64 * 32];
    uint8_t keypad[16];


};

#endif //UNTITLED_CHIP8_H