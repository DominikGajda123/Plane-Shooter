#pragma once

#include "PociskBazowy.h"

class PociskWroga : public PociskBazowy
{
public:
    PociskWroga(sf::Vector2f pozycja);
    PociskWroga(sf::Vector2f pozycja, sf::Vector2f kierunek, float szybkosc);

    bool pozaEkranem() const;
};