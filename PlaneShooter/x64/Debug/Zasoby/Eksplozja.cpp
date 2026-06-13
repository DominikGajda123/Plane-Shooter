#include "Eksplozja.h"

Eksplozja::Eksplozja(sf::Vector2f pozycja, const std::vector<sf::Texture*>& tekstury)
    : sprite(*tekstury[0]), klatki(tekstury)
{
    ustawOriginNaSrodek();
    sprite.setPosition(pozycja);
    sprite.setScale({ 0.8f, 0.8f });
}

void Eksplozja::aktualizacja(float deltaCzas)
{
    if (zakonczono)
        return;

    czas += deltaCzas;

    if (czas < czasKlatki)
        return;

    czas = 0.f;
    aktualnaKlatka++;

    if (aktualnaKlatka >= static_cast<int>(klatki.size()))
    {
        zakonczono = true;
        return;
    }

    sprite.setTexture(*klatki[aktualnaKlatka], true);
    ustawOriginNaSrodek();
}

void Eksplozja::ustawOriginNaSrodek()
{
    const sf::FloatRect granice = sprite.getGlobalBounds();
    sprite.setOrigin({ granice.size.x / 2.f, granice.size.y / 2.f });
}

void Eksplozja::rysuj(sf::RenderWindow& okno) const
{
    if (!zakonczono)
    {
        okno.draw(sprite);
    }
}

bool Eksplozja::czyZakonczona() const
{
    return zakonczono;
}