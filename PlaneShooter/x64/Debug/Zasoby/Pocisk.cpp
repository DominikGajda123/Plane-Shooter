#include "Pocisk.h"

namespace
{
    constexpr float PREDKOSC_POCISKU = 500.f;
    constexpr float PROMIEN_POCISKU = 5.f;
    constexpr float GORNA_GRANICA_EKRANU = -20.f;
}

Pocisk::Pocisk(sf::Vector2f pozycja, float przesuniecieX, float mnoznikPredkosci)
    : PociskBazowy(
        { pozycja.x + przesuniecieX, pozycja.y },
        PROMIEN_POCISKU,
        sf::Color::Red,
        { 0.f, -PREDKOSC_POCISKU * mnoznikPredkosci }
    )
{
}

bool Pocisk::pozaEkranem() const
{
    return ksztalt.getPosition().y < GORNA_GRANICA_EKRANU;
}