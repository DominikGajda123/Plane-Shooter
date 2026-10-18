# Plane-Shooter
Programowanie obiektowe 1 - projekt autor Dominik Gajda i Kamil Ruzik

Kamień Milowy 1 (Termin: 08.05.2026)

Konfiguracja środowiska: SFML 3.1.0

Logika poruszania się: Implementacja klasy gracza (Player) z obsługą klawiatury (ruch w 8 kierunkach).

System renderowania: Wyświetlanie tła (scrollowanie mapy) oraz modelu samolotu gracza

Podstawowy system strzelania: Generowanie pocisków po naciśnięciu spacji.

Zarządzanie zasobami: Klasa ładująca tekstury i dźwięki.

__________________________________________________________

Instrukcja instalacji SFML:

https://www.youtube.com/watch?v=4k9ba8PIvKU

Z folderu SFML można usunąć wszystko oprócz include i lib

___________________________________________________________

#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Engine
{
private:
    sf::RenderWindow window;
    sf::Color backgroundColor;
    sf::Clock clock;

    unsigned int width;
    unsigned int height;
    unsigned int fps;

    bool fullscreen;

    void processEvents();
    void update(float deltaTime);
    void render();

public:
    Engine(unsigned int width = 800,
           unsigned int height = 600,
           unsigned int fps = 60,
           bool fullscreen = false);

    void run();
};



#include "Engine.h"
#include <iostream>
#include <optional>
#include <exception>

Engine::Engine(unsigned int width,
               unsigned int height,
               unsigned int fps,
               bool fullscreen)
    : width(width),
      height(height),
      fps(fps),
      fullscreen(fullscreen),
      backgroundColor(sf::Color::Black)
{
    sf::State state = fullscreen
        ? sf::State::Fullscreen
        : sf::State::Windowed;

    window.create(
        sf::VideoMode({ width, height }),
        "Silnik 2D - SFML",
        sf::Style::Default,
        state
    );

    window.setFramerateLimit(fps);

    std::cout << "Silnik uruchomiony!" << std::endl;
}

void Engine::processEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (const auto* key =
            event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Escape)
                window.close();

            if (key->code == sf::Keyboard::Key::R)
                backgroundColor = sf::Color::Red;

            if (key->code == sf::Keyboard::Key::G)
                backgroundColor = sf::Color::Green;

            if (key->code == sf::Keyboard::Key::B)
                backgroundColor = sf::Color::Blue;

            if (key->code == sf::Keyboard::Key::Space)
                backgroundColor = sf::Color::Black;
        }

        if (const auto* mouse =
            event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                backgroundColor = sf::Color::Yellow;

                std::cout << "Klikniecie myszy: "
                          << mouse->position.x << ", "
                          << mouse->position.y << std::endl;
            }

            if (mouse->button == sf::Mouse::Button::Right)
            {
                backgroundColor = sf::Color::Cyan;
            }
        }
    }
}

void Engine::update(float deltaTime)
{
    // Tutaj bedzie logika silnika.
    // deltaTime - czas od poprzedniej klatki.
    (void)deltaTime;
}

void Engine::render()
{
    window.clear(backgroundColor);

    // Tutaj bedziemy rysowac figury.

    window.display();
}

void Engine::run()
{
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);

        if (window.isOpen())
            render();
    }

    std::cout << "Silnik zamkniety." << std::endl;
}

#include "Engine.h"
#include <iostream>
#include <exception>

int main()
{
    try
    {
        Engine engine(800, 600, 60, false);
        engine.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Blad: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
