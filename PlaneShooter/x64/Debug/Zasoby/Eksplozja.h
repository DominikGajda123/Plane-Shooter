#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Eksplozja
{
private:
    sf::Sprite sprite;
    std::vector<sf::Texture*> klatki;
    float czas = 0.f;
    float czasKlatki = 0.08f;
    int aktualnaKlatka = 0;
    bool zakonczono = false;

    void ustawOriginNaSrodek();

public:
    Eksplozja(sf::Vector2f pozycja, const std::vector<sf::Texture*>& tekstury);

    void aktualizacja(float deltaCzas);
    void rysuj(sf::RenderWindow& okno) const;
    bool czyZakonczona() const;
};