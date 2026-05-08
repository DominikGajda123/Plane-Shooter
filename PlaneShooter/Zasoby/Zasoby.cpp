#include "Zasoby.h"

#include <iostream>

void Zasoby::wczytajTeksture(
    const std::string& nazwa,
    const std::string& sciezka
)
{
    if (!tekstury[nazwa].loadFromFile(sciezka))
    {
        std::cout << "Nie mozna wczytac tekstury: "
            << sciezka << std::endl;
    }
}

sf::Texture& Zasoby::pobierzTeksture(
    const std::string& nazwa
)
{
    return tekstury[nazwa];
}