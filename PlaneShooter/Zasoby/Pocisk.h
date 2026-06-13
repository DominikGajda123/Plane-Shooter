#pragma once

#include "PociskBazowy.h"

class Pocisk : public PociskBazowy
{
public:
    Pocisk(sf::Vector2f pozycja, float przesuniecieX = 20.f, float mnoznikPredkosci = 1.f);

    bool pozaEkranem() const;
};