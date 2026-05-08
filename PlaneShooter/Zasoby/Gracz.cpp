#include "Gracz.h"

Gracz::Gracz(sf::Texture& tekstura)
    : tekstura(&tekstura),
    sprite(tekstura)
{
    sprite.setPosition({ 400.f, 500.f });

    sprite.setScale({ 0.5f, 0.5f });
}

void Gracz::obslugaRuchu(float deltaCzas)
{
    sf::Vector2f ruch = { 0.f, 0.f };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        ruch.y -= predkosc * deltaCzas;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        ruch.y += predkosc * deltaCzas;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        ruch.x -= predkosc * deltaCzas;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        ruch.x += predkosc * deltaCzas;

    sprite.move(ruch);

    sf::Vector2f pozycja = sprite.getPosition();

    if (pozycja.x < 0)
        pozycja.x = 0;

    if (pozycja.x > 750)
        pozycja.x = 750;

    if (pozycja.y < 0)
        pozycja.y = 0;

    if (pozycja.y > 550)
        pozycja.y = 550;

    sprite.setPosition(pozycja);
}

void Gracz::rysuj(sf::RenderWindow& okno)
{
    okno.draw(sprite);
}

sf::Vector2f Gracz::pobierzPozycje() const
{
    return sprite.getPosition();
}