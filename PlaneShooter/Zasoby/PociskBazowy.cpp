#include "PociskBazowy.h"

PociskBazowy::PociskBazowy(sf::Vector2f pozycja, float promien, sf::Color kolor, sf::Vector2f predkosc)
    : predkosc(predkosc)
{
    ksztalt.setRadius(promien);
    ksztalt.setOrigin({ promien, promien });
    ksztalt.setFillColor(kolor);
    ksztalt.setPosition(pozycja);
}

void PociskBazowy::aktualizacja(float deltaCzas)
{
    ksztalt.move({ predkosc.x * deltaCzas, predkosc.y * deltaCzas });
}

void PociskBazowy::rysuj(sf::RenderWindow& okno) const
{
    if (aktywny)
    {
        okno.draw(ksztalt);
    }
}

sf::FloatRect PociskBazowy::pobierzGranice() const
{
    return ksztalt.getGlobalBounds();
}

bool PociskBazowy::czyAktywny() const
{
    return aktywny;
}

void PociskBazowy::dezaktywuj()
{
    aktywny = false;
}