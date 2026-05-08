#pragma once

#include <SFML/Graphics.hpp>

class Gracz
{
private:
    sf::Texture* tekstura = nullptr;

    sf::Sprite sprite;

    float predkosc = 300.f;

public:
    Gracz(sf::Texture& tekstura);

    void obslugaRuchu(float deltaCzas);

    void rysuj(sf::RenderWindow& okno);

    sf::Vector2f pobierzPozycje() const;
};