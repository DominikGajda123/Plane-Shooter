#include "Gracz.h"

#include <algorithm>

namespace
{
    constexpr float MIN_X = 0.f;
    constexpr float MAX_X = 750.f;
    constexpr float MIN_Y = 0.f;
    constexpr float MAX_Y = 550.f;
    constexpr float PROMIEN_TARCZY = 34.f;
}

Gracz::Gracz(sf::Texture& tekstura)
    : sprite(tekstura)
{
    sprite.setPosition({ 400.f, 500.f });
    sprite.setScale({ 0.5f, 0.5f });

    obwodkaTarczy.setRadius(PROMIEN_TARCZY);
    obwodkaTarczy.setOrigin({ PROMIEN_TARCZY, PROMIEN_TARCZY });
    obwodkaTarczy.setFillColor(sf::Color(0, 120, 255, 55));
    obwodkaTarczy.setOutlineColor(sf::Color::Cyan);
    obwodkaTarczy.setOutlineThickness(3.f);
    aktualizujPozycjeTarczy();
}

void Gracz::obslugaRuchu(float deltaCzas)
{
    sf::Vector2f ruch = { 0.f, 0.f };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) ruch.y -= predkosc * deltaCzas;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) ruch.y += predkosc * deltaCzas;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) ruch.x -= predkosc * deltaCzas;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) ruch.x += predkosc * deltaCzas;

    sprite.move(ruch);
    ograniczDoEkranu();
    aktualizujPozycjeTarczy();
}

void Gracz::ograniczDoEkranu()
{
    sf::Vector2f pozycja = sprite.getPosition();
    pozycja.x = std::clamp(pozycja.x, MIN_X, MAX_X);
    pozycja.y = std::clamp(pozycja.y, MIN_Y, MAX_Y);
    sprite.setPosition(pozycja);
}

void Gracz::rysuj(sf::RenderWindow& okno) const
{
    okno.draw(sprite);

    if (tarcza)
    {
        okno.draw(obwodkaTarczy);
    }
}

void Gracz::aktualizujPozycjeTarczy()
{
    const sf::FloatRect granice = sprite.getGlobalBounds();

    obwodkaTarczy.setPosition({
        granice.position.x + granice.size.x / 2.f,
        granice.position.y + granice.size.y / 2.f
    });
}

void Gracz::otrzymajObrazenia(int dmg)
{
    if (tarcza)
    {
        tarcza = false;
        return;
    }

    zycia = std::max(0, zycia - dmg);
}

void Gracz::ulecz(int ile)
{
    zycia = std::min(maxZycia, zycia + ile);
}

void Gracz::wlaczTarcze()
{
    aktualizujPozycjeTarczy();
    tarcza = true;
}

bool Gracz::maTarcze() const { return tarcza; }
int Gracz::pobierzZycia() const { return zycia; }
int Gracz::pobierzMaxZycia() const { return maxZycia; }
sf::Vector2f Gracz::pobierzPozycje() const { return sprite.getPosition(); }
sf::FloatRect Gracz::pobierzGranice() const { return sprite.getGlobalBounds(); }