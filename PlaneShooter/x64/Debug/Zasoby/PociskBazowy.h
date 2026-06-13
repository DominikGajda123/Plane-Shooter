#pragma once

#include <SFML/Graphics.hpp>

class PociskBazowy
{
protected:
    sf::CircleShape ksztalt;
    sf::Vector2f predkosc;
    bool aktywny = true;

public:
    PociskBazowy(sf::Vector2f pozycja, float promien, sf::Color kolor, sf::Vector2f predkosc);

    void aktualizacja(float deltaCzas);
    void rysuj(sf::RenderWindow& okno) const;
    sf::FloatRect pobierzGranice() const;
    bool czyAktywny() const;
    void dezaktywuj();
};