#pragma once

#include <SFML/Graphics.hpp>

class Boss
{
private:
    sf::Sprite sprite;
    float predkoscX = 95.f;
    int hp = 80;
    int maxHp = 80;

public:
    Boss(sf::Texture& tekstura);

    void aktualizacja(float deltaCzas);
    void rysuj(sf::RenderWindow& okno) const;
    void otrzymajObrazenia(int dmg);

    bool czyMartwy() const;
    sf::FloatRect pobierzGranice() const;
    sf::Vector2f pobierzSrodek() const;
    int pobierzHp() const;
    int pobierzMaxHp() const;
};