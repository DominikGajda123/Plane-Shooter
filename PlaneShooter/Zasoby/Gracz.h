#pragma once

#include <SFML/Graphics.hpp>

class Gracz
{
private:
    sf::Sprite sprite;
    sf::CircleShape obwodkaTarczy;
    float predkosc = 300.f;
    int zycia = 3;
    int maxZycia = 3;
    bool tarcza = false;

    void ograniczDoEkranu();
    void aktualizujPozycjeTarczy();

public:
    Gracz(sf::Texture& tekstura);

    void obslugaRuchu(float deltaCzas);
    void rysuj(sf::RenderWindow& okno) const;
    void otrzymajObrazenia(int dmg = 1);
    void ulecz(int ile);
    void wlaczTarcze();

    bool maTarcze() const;
    int pobierzZycia() const;
    int pobierzMaxZycia() const;
    sf::Vector2f pobierzPozycje() const;
    sf::FloatRect pobierzGranice() const;
};