#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <string>

class Zasoby
{
private:
    std::map<std::string, sf::Texture> tekstury;

public:
    void wczytajTeksture(const std::string& nazwa, const std::string& sciezka);
    sf::Texture& pobierzTeksture(const std::string& nazwa);
};
