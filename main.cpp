#include <iostream>
#include <SFML/Graphics.hpp>
#include <optional>
#include "Chip8.h"
#include <SFML/Audio.hpp>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    Chip8 chip8;
    chip8.loadROM("../Pong.ch8"); //Feel free to change path!

    const unsigned int SCALE = 20;

    sf::RenderWindow window(sf::VideoMode({64u * SCALE, 32u * SCALE}), sf::String("CHIP-8 Emulator"));
    sf::RectangleShape pixelRect(sf::Vector2f(static_cast<float>(SCALE), static_cast<float>(SCALE)));
    pixelRect.setFillColor(sf::Color::White);

    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("../sound.wav")) {
        std::cout << "Error loading sound.wav" << std::endl;
    }

    sf::Sound chip8Sound(soundBuffer);
    chip8Sound.setLooping(true);

    sf::Clock cpuClock;
    sf::Clock timerClock;


    while (window.isOpen()) {


        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }


            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                int chip8Key = -1;
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Num1: chip8Key = 0x1; break;
                    case sf::Keyboard::Key::Num2: chip8Key = 0x2; break;
                    case sf::Keyboard::Key::Num3: chip8Key = 0x3; break;
                    case sf::Keyboard::Key::Num4: chip8Key = 0xC; break;
                    case sf::Keyboard::Key::Q:    chip8Key = 0x4; break;
                    case sf::Keyboard::Key::W:    chip8Key = 0x5; break;
                    case sf::Keyboard::Key::E:    chip8Key = 0x6; break;
                    case sf::Keyboard::Key::R:    chip8Key = 0xD; break;
                    case sf::Keyboard::Key::A:    chip8Key = 0x7; break;
                    case sf::Keyboard::Key::S:    chip8Key = 0x8; break;
                    case sf::Keyboard::Key::D:    chip8Key = 0x9; break;
                    case sf::Keyboard::Key::F:    chip8Key = 0xE; break;
                    case sf::Keyboard::Key::Y:    chip8Key = 0xA; break;
                    case sf::Keyboard::Key::X:    chip8Key = 0x0; break;
                    case sf::Keyboard::Key::C:    chip8Key = 0xB; break;
                    case sf::Keyboard::Key::V:    chip8Key = 0xF; break;
                    default: break;
                }
                if (chip8Key != -1) chip8.setKeyState(chip8Key, true);
            }


            if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
                int chip8Key = -1;
                switch (keyReleased->code) {
                    case sf::Keyboard::Key::Num1: chip8Key = 0x1; break;
                    case sf::Keyboard::Key::Num2: chip8Key = 0x2; break;
                    case sf::Keyboard::Key::Num3: chip8Key = 0x3; break;
                    case sf::Keyboard::Key::Num4: chip8Key = 0xC; break;
                    case sf::Keyboard::Key::Q:    chip8Key = 0x4; break;
                    case sf::Keyboard::Key::W:    chip8Key = 0x5; break;
                    case sf::Keyboard::Key::E:    chip8Key = 0x6; break;
                    case sf::Keyboard::Key::R:    chip8Key = 0xD; break;
                    case sf::Keyboard::Key::A:    chip8Key = 0x7; break;
                    case sf::Keyboard::Key::S:    chip8Key = 0x8; break;
                    case sf::Keyboard::Key::D:    chip8Key = 0x9; break;
                    case sf::Keyboard::Key::F:    chip8Key = 0xE; break;
                    case sf::Keyboard::Key::Y:    chip8Key = 0xA; break;
                    case sf::Keyboard::Key::X:    chip8Key = 0x0; break;
                    case sf::Keyboard::Key::C:    chip8Key = 0xB; break;
                    case sf::Keyboard::Key::V:    chip8Key = 0xF; break;
                    default: break;
                }
                if (chip8Key != -1) chip8.setKeyState(chip8Key, false);
            }
        }



        if (cpuClock.getElapsedTime().asMilliseconds() >= 2) {
            chip8.cycle();
            cpuClock.restart();
        }

        if (timerClock.getElapsedTime().asMilliseconds() >= 16) {

            if (chip8.getSoundTimer() > 0) {
                if (chip8Sound.getStatus() != sf::Sound::Status::Playing) {
                    chip8Sound.play();
                }
            }
            else {
                if (chip8Sound.getStatus() == sf::Sound::Status::Playing) {
                    chip8Sound.stop();
                }
            }

            chip8.updateTimers();
            timerClock.restart();
        }


        window.clear(sf::Color::Black);

        for (unsigned int y = 0; y < 32; y++) {
            for (unsigned int x = 0; x < 64; x++) {
                if (chip8.getDisplay()[x + (y * 64)] == 1) {
                    pixelRect.setPosition({static_cast<float>(x * SCALE), static_cast<float>(y * SCALE)});
                    window.draw(pixelRect);
                }
            }
        }
        window.display();

    }

    return 0;
}