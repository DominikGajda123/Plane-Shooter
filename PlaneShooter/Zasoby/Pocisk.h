#pragma once

#include <SFML/Graphics.hpp>

class Pocisk
{
private:
    sf::CircleShape ksztalt;

    float predkosc = 500.f;

public:
    Pocisk(sf::Vector2f pozycja);

    void aktualizacja(float deltaCzas);

    void rysuj(sf::RenderWindow& okno);
};