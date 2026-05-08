#include "Gra.h"

#include <optional>

Gra::Gra()
    : okno(sf::VideoMode({ 800, 600 }), "Plane Shooter")
{
    zasoby.wczytajTeksture("gracz", "zasoby/samolot.png");
    zasoby.wczytajTeksture("tlo", "zasoby/tlo.png");

    gracz = std::make_unique<Gracz>(
        zasoby.pobierzTeksture("gracz")
    );

    tlo = std::make_unique<sf::Sprite>(
        zasoby.pobierzTeksture("tlo")
    );
}

void Gra::uruchom()
{
    sf::Clock zegar;

    while (okno.isOpen())
    {
        float deltaCzas = zegar.restart().asSeconds();

        obslugaZdarzen();

        aktualizacja(deltaCzas);

        renderowanie();
    }
}

void Gra::obslugaZdarzen()
{
    while (const std::optional zdarzenie = okno.pollEvent())
    {
        if (zdarzenie->is<sf::Event::Closed>())
        {
            okno.close();
        }
    }
}

void Gra::aktualizacja(float deltaCzas)
{
    licznikStrzalu += deltaCzas;

    gracz->obslugaRuchu(deltaCzas);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)
        && licznikStrzalu >= cooldownStrzalu)
    {
        pociski.emplace_back(
            gracz->pobierzPozycje()
        );

        licznikStrzalu = 0.f;
    }

    for (auto& pocisk : pociski)
    {
        pocisk.aktualizacja(deltaCzas);
    }
}

void Gra::renderowanie()
{
    okno.clear();

    okno.draw(*tlo);

    gracz->rysuj(okno);

    for (auto& pocisk : pociski)
    {
        pocisk.rysuj(okno);
    }

    okno.display();
}