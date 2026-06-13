#include "Ulepszenie.h"

namespace
{
    constexpr float PROMIEN_ULEPSZENIA = 14.f;
    constexpr float DOLNA_GRANICA_EKRANU = 630.f;
}

Ulepszenie::Ulepszenie(sf::Vector2f pozycja, TypUlepszenia typ)
    : typ(typ)
{
    ksztalt.setRadius(PROMIEN_ULEPSZENIA);
    ksztalt.setOrigin({ PROMIEN_ULEPSZENIA, PROMIEN_ULEPSZENIA });
    ksztalt.setPosition(pozycja);
    ksztalt.setOutlineColor(sf::Color::White);
    ksztalt.setOutlineThickness(2.f);

    switch (typ)
    {
    case TypUlepszenia::Szybkostrzelnosc: ksztalt.setFillColor(sf::Color::Magenta); break;
    case TypUlepszenia::Tarcza: ksztalt.setFillColor(sf::Color::Cyan); break;
    case TypUlepszenia::PodwojnyStrzal: ksztalt.setFillColor(sf::Color::Green); break;
    case TypUlepszenia::DodatkoweZycie: ksztalt.setFillColor(sf::Color::Red); break;
    }
}

void Ulepszenie::aktualizacja(float deltaCzas)
{
    ksztalt.move({ 0.f, predkosc * deltaCzas });
}

void Ulepszenie::rysuj(sf::RenderWindow& okno) const
{
    if (aktywne)
    {
        okno.draw(ksztalt);
    }
}

sf::FloatRect Ulepszenie::pobierzGranice() const
{
    return ksztalt.getGlobalBounds();
}

bool Ulepszenie::pozaEkranem() const
{
    return ksztalt.getPosition().y > DOLNA_GRANICA_EKRANU;
}
