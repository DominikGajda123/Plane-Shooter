#include "Pocisk.h"

Pocisk::Pocisk(sf::Vector2f pozycja)
{
    ksztalt.setRadius(5.f);

    ksztalt.setFillColor(sf::Color::Red);

    ksztalt.setPosition(pozycja);
}

void Pocisk::aktualizacja(float deltaCzas)
{
    ksztalt.move({ 0.f, -predkosc * deltaCzas });
}

void Pocisk::rysuj(sf::RenderWindow& okno)
{
    okno.draw(ksztalt);
}