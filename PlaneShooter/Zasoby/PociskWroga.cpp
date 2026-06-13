#include "PociskWroga.h"

#include <cmath>

namespace
{
    constexpr float PREDKOSC_POCISKU_WROGA = 260.f;
    constexpr float PROMIEN_POCISKU_WROGA = 6.f;
    constexpr float PROMIEN_POCISKU_BOSSA = 7.f;
    constexpr float MARGINES_POZA_EKRANEM = 40.f;
    constexpr float SZEROKOSC_OKNA = 800.f;
    constexpr float WYSOKOSC_OKNA = 600.f;

    sf::Vector2f normalizuj(sf::Vector2f kierunek, float szybkosc)
    {
        const float dlugosc = std::sqrt(kierunek.x * kierunek.x + kierunek.y * kierunek.y);

        if (dlugosc == 0.f)
        {
            return { 0.f, szybkosc };
        }

        return {
            kierunek.x / dlugosc * szybkosc,
            kierunek.y / dlugosc * szybkosc
        };
    }
}

PociskWroga::PociskWroga(sf::Vector2f pozycja)
    : PociskBazowy(
        { pozycja.x, pozycja.y + 20.f },
        PROMIEN_POCISKU_WROGA,
        sf::Color::Yellow,
        { 0.f, PREDKOSC_POCISKU_WROGA }
    )
{
}

PociskWroga::PociskWroga(sf::Vector2f pozycja, sf::Vector2f kierunek, float szybkosc)
    : PociskBazowy(
        pozycja,
        PROMIEN_POCISKU_BOSSA,
        sf::Color::Red,
        normalizuj(kierunek, szybkosc)
    )
{
}

bool PociskWroga::pozaEkranem() const
{
    const sf::Vector2f pozycja = ksztalt.getPosition();

    return pozycja.x < -MARGINES_POZA_EKRANEM
        || pozycja.x > SZEROKOSC_OKNA + MARGINES_POZA_EKRANEM
        || pozycja.y < -MARGINES_POZA_EKRANEM
        || pozycja.y > WYSOKOSC_OKNA + MARGINES_POZA_EKRANEM;
}