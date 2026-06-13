#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <optional>
#include <random>
#include <vector>

#include "Boss.h"
#include "Eksplozja.h"
#include "Gracz.h"
#include "Pocisk.h"
#include "PociskWroga.h"
#include "Przeciwnik.h"
#include "Ulepszenie.h"
#include "Zasoby.h"

class Gra
{
private:
    sf::RenderWindow okno;
    Zasoby zasoby;

    std::optional<Gracz> gracz;
    std::optional<Boss> boss;
    std::optional<sf::Sprite> tlo;
    std::optional<sf::Text> tekstScore;
    std::optional<sf::Text> tekstGameOver;
    std::optional<sf::Sound> dzwiekStrzalu;
    std::optional<sf::Sound> dzwiekWybuchu;
    std::optional<sf::Sound> dzwiekKonca;

    std::vector<Pocisk> pociski;
    std::vector<PociskWroga> pociskiWrogow;
    std::vector<Enemy> przeciwnicy;
    std::vector<Ulepszenie> ulepszenia;
    std::vector<Eksplozja> eksplozje;
    std::vector<sf::Sprite> serduszka;
    std::vector<sf::Texture*> klatkiEksplozji;

    sf::Music muzyka;
    sf::SoundBuffer buforStrzalu;
    sf::SoundBuffer buforWybuchu;
    sf::SoundBuffer buforKonca;
    sf::Font font;

    std::mt19937 rng;

    float cooldownStrzalu = 0.2f;
    float licznikStrzalu = 0.f;
    float cooldownSpawnu = 1.2f;
    float licznikSpawnu = 0.f;
    float cooldownKolizji = 0.8f;
    float licznikKolizji = 0.f;
    float cooldownStrzaluWroga = 1.3f;
    float licznikStrzaluWroga = 0.f;
    float cooldownStrzaluBossa = 0.75f;
    float licznikStrzaluBossa = 0.f;

    bool szybkiePociski = false;
    bool podwojnyStrzal = false;
    int score = 0;
    int nastepnyBossScore = 1000;
    bool gameOver = false;
    bool dzwiekKoncaOdtworzony = false;

    void wczytajZasoby();
    void przygotujObiekty();
    void przygotujAudio();
    void przygotujUI();

    void obslugaZdarzen();
    void aktualizacja(float deltaCzas);
    void renderowanie();

    void aktualizujLiczniki(float deltaCzas);
    void aktualizujObiekty(float deltaCzas);
    void sprawdzKoniecGry();
    void aktualizujUI();

    void strzel();
    void spawnPrzeciwnika();
    void spawnUlepszenia(sf::Vector2f pozycja);
    void strzalWrogow();
    void sprawdzStartBossa();
    void strzalBossa();

    bool czyUlepszenieDostepne(TypUlepszenia typ) const;
    void aktywujUlepszenie(TypUlepszenia typ);
    void dodajEksplozje(sf::FloatRect granice);

    void sprawdzKolizje();
    void sprawdzKolizjePociskowZPrzeciwnikami();
    void sprawdzKolizjePociskowZBossem();
    void sprawdzKolizjeUlepszen();
    void sprawdzKolizjePociskowWrogow();
    void sprawdzKolizjeGraczaZPrzeciwnikami();

    void usunNieaktywneObiekty();
    void usunPociski();
    void usunPociskiWrogow();
    void usunUlepszenia();
    void usunPrzeciwnikow();
    void usunEksplozje();

    void rysujPasekZyciaBossa();
    void rysujPociski();
    void rysujPrzeciwnikow();
    void rysujEfekty();
    void rysujUlepszenia();
    void rysujUI();

    int losujInt(int min, int max);

public:
    Gra();

    void uruchom();
};