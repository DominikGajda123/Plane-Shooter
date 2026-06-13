#pragma once

#include <SFML/Graphics.hpp>

class Enemy
{
public:
    enum class Typ
    {
        Meteoryt,
        Statek
    };

private:
    sf::Sprite sprite;
    Typ typ;
    float predkosc = 150.f;
    int hp = 1;

public:
    Enemy(sf::Vector2f pozycja, Typ typ, sf::Texture& tekstura);

    void aktualizacja(float deltaCzas, sf::Vector2f pozycjaGracza);
    void rysuj(sf::RenderWindow& okno) const;
    void otrzymajObrazenia(int dmg);

    bool czyMartwy() const;
    bool pozaEkranem() const;
    sf::FloatRect pobierzGranice() const;
    sf::Vector2f pobierzPozycjeStrzalu() const;
    Typ pobierzTyp() const;
};