#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

#include "Gracz.h"
#include "Pocisk.h"
#include "Zasoby.h"

class Gra
{
private:
    sf::RenderWindow okno;

    Zasoby zasoby;

    std::unique_ptr<Gracz> gracz;

    std::vector<Pocisk> pociski;

    std::unique_ptr<sf::Sprite> tlo;

    float cooldownStrzalu = 0.2f;
    float licznikStrzalu = 0.f;

    void obslugaZdarzen();

    void aktualizacja(float deltaCzas);

    void renderowanie();

public:
    Gra();

    void uruchom();
};