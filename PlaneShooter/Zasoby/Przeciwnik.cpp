#include "Przeciwnik.h"

namespace
{
    constexpr float PREDKOSC_METEORYTU = 160.f;
    constexpr float PREDKOSC_STATKU = 220.f;
    constexpr float PREDKOSC_SLEDZENIA_GRACZA = 90.f;
    constexpr float DOLNA_GRANICA_EKRANU = 650.f;
}

Enemy::Enemy(sf::Vector2f pozycja, Typ typ, sf::Texture& tekstura)
    : sprite(tekstura), typ(typ)
{
    if (typ == Typ::Meteoryt)
    {
        predkosc = PREDKOSC_METEORYTU;
        hp = 2;
        sprite.setScale({ 0.06f, 0.06f });
    }
    else
    {
        predkosc = PREDKOSC_STATKU;
        hp = 1;
        sprite.setScale({ 0.05f, 0.05f });
    }

    sprite.setPosition(pozycja);
}

void Enemy::aktualizacja(float deltaCzas, sf::Vector2f pozycjaGracza)
{
    sf::Vector2f ruch = { 0.f, predkosc * deltaCzas };

    if (typ == Typ::Statek)
    {
        if (pozycjaGracza.x < sprite.getPosition().x) ruch.x -= PREDKOSC_SLEDZENIA_GRACZA * deltaCzas;
        else if (pozycjaGracza.x > sprite.getPosition().x) ruch.x += PREDKOSC_SLEDZENIA_GRACZA * deltaCzas;
    }

    sprite.move(ruch);
}

void Enemy::rysuj(sf::RenderWindow& okno) const
{
    okno.draw(sprite);
}

void Enemy::otrzymajObrazenia(int dmg)
{
    hp -= dmg;
}

bool Enemy::czyMartwy() const { return hp <= 0; }
bool Enemy::pozaEkranem() const { return sprite.getPosition().y > DOLNA_GRANICA_EKRANU; }
sf::FloatRect Enemy::pobierzGranice() const { return sprite.getGlobalBounds(); }

sf::Vector2f Enemy::pobierzPozycjeStrzalu() const
{
    const sf::FloatRect granice = sprite.getGlobalBounds();

    return {
        granice.position.x + granice.size.x / 2.f,
        granice.position.y + granice.size.y
    };
}

Enemy::Typ Enemy::pobierzTyp() const { return typ; }