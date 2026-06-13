#pragma once

#include <SFML/Graphics.hpp>

enum class TypUlepszenia
{
    Szybkostrzelnosc,
    Tarcza,
    PodwojnyStrzal,
    DodatkoweZycie
};

struct Ulepszenie
{
    TypUlepszenia typ;
    sf::CircleShape ksztalt;
    float predkosc = 110.f;
    bool aktywne = true;

    Ulepszenie(sf::Vector2f pozycja, TypUlepszenia typ);

    void aktualizacja(float deltaCzas);
    void rysuj(sf::RenderWindow& okno) const;
    sf::FloatRect pobierzGranice() const;
    bool pozaEkranem() const;
};
