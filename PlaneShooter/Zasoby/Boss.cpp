#include "Boss.h"

namespace
{
    constexpr float DOCELOWA_POZYCJA_Y = 140.f;
    constexpr float PREDKOSC_WEJSCIA = 80.f;
    constexpr float LEWA_GRANICA = 10.f;
    constexpr float PRAWA_GRANICA = 790.f;
}

Boss::Boss(sf::Texture& tekstura)
    : sprite(tekstura)
{
    sprite.setScale({ 0.45f, 0.45f });

    const sf::FloatRect granice = sprite.getGlobalBounds();
    sprite.setOrigin({ granice.size.x / 2.f, granice.size.y / 2.f });
    sprite.setPosition({ 400.f, -120.f });
}

void Boss::aktualizacja(float deltaCzas)
{
    sf::Vector2f ruch = { predkoscX * deltaCzas, 0.f };

    if (sprite.getPosition().y < DOCELOWA_POZYCJA_Y)
    {
        ruch.y = PREDKOSC_WEJSCIA * deltaCzas;
    }

    sprite.move(ruch);

    const sf::FloatRect granice = sprite.getGlobalBounds();

    if (granice.position.x < LEWA_GRANICA || granice.position.x + granice.size.x > PRAWA_GRANICA)
    {
        predkoscX = -predkoscX;
    }
}

void Boss::rysuj(sf::RenderWindow& okno) const
{
    okno.draw(sprite);
}

void Boss::otrzymajObrazenia(int dmg)
{
    hp -= dmg;
}

bool Boss::czyMartwy() const { return hp <= 0; }
sf::FloatRect Boss::pobierzGranice() const { return sprite.getGlobalBounds(); }
sf::Vector2f Boss::pobierzSrodek() const { return sprite.getPosition(); }
int Boss::pobierzHp() const { return hp; }
int Boss::pobierzMaxHp() const { return maxHp; }