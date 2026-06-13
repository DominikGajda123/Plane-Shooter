#include "Gra.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <string>

namespace
{
    constexpr unsigned SZEROKOSC_OKNA = 800;
    constexpr unsigned WYSOKOSC_OKNA = 600;
    constexpr float MIN_COOLDOWN_SPAWNU = 0.35f;
    constexpr float START_COOLDOWN_SPAWNU = 1.2f;
    constexpr float SKALOWANIE_COOLDOWNU = 1000.f;
    constexpr float PI = 3.14159265f;
    constexpr int LICZBA_POCISKOW_BOSSA = 20;

    bool kolizja(const sf::FloatRect& a, const sf::FloatRect& b)
    {
        return a.findIntersection(b).has_value();
    }

    sf::Vector2f srodek(const sf::FloatRect& granice)
    {
        return {
            granice.position.x + granice.size.x / 2.f,
            granice.position.y + granice.size.y / 2.f
        };
    }

    template <typename T, typename Predykat>
    void usunJesli(std::vector<T>& lista, Predykat predykat)
    {
        lista.erase(std::remove_if(lista.begin(), lista.end(), predykat), lista.end());
    }
}

Gra::Gra()
    : okno(sf::VideoMode({ SZEROKOSC_OKNA, WYSOKOSC_OKNA }), "Plane Shooter"),
      rng(std::random_device{}())
{
    wczytajZasoby();
    przygotujAudio();
    przygotujObiekty();
    przygotujUI();
}

void Gra::wczytajZasoby()
{
    zasoby.wczytajTeksture("gracz", "zasoby/samolot.png");
    zasoby.wczytajTeksture("tlo", "zasoby/tlo.png");
    zasoby.wczytajTeksture("serce", "zasoby/serce.png");
    zasoby.wczytajTeksture("meteoryt", "zasoby/meteoryt.png");
    zasoby.wczytajTeksture("wrog", "zasoby/wrog.png");
    zasoby.wczytajTeksture("boss", "zasoby/boss.png");
    zasoby.wczytajTeksture("wybuch1", "zasoby/wybuch1.png");
    zasoby.wczytajTeksture("wybuch2", "zasoby/wybuch2.png");
    zasoby.wczytajTeksture("wybuch3", "zasoby/wybuch3.png");

    klatkiEksplozji = {
        &zasoby.pobierzTeksture("wybuch1"),
        &zasoby.pobierzTeksture("wybuch2"),
        &zasoby.pobierzTeksture("wybuch3")
    };
}

void Gra::przygotujAudio()
{
    if (muzyka.openFromFile("zasoby/Chiptronical.ogg"))
    {
        muzyka.setLooping(true);
        muzyka.setVolume(35.f);
        muzyka.play();
    }

    if (buforStrzalu.loadFromFile("zasoby/strzal.mp3"))
    {
        dzwiekStrzalu.emplace(buforStrzalu);
        dzwiekStrzalu->setVolume(70.f);
    }

    if (buforWybuchu.loadFromFile("zasoby/wybuch.wav"))
    {
        dzwiekWybuchu.emplace(buforWybuchu);
        dzwiekWybuchu->setVolume(80.f);
    }

    if (buforKonca.loadFromFile("zasoby/koniec.wav"))
    {
        dzwiekKonca.emplace(buforKonca);
        dzwiekKonca->setVolume(90.f);
    }
}

void Gra::przygotujObiekty()
{
    gracz.emplace(zasoby.pobierzTeksture("gracz"));
    tlo.emplace(zasoby.pobierzTeksture("tlo"));

    serduszka.reserve(3);
    for (int i = 0; i < 3; ++i)
    {
        sf::Sprite serce(zasoby.pobierzTeksture("serce"));
        serce.setPosition({ 10.f + i * 42.f, 10.f });
        serce.setScale({ 0.08f, 0.08f });
        serduszka.push_back(std::move(serce));
    }
}

void Gra::przygotujUI()
{
    font.openFromFile("zasoby/arial.ttf");

    tekstScore.emplace(font, "Score: 0", 24);
    tekstGameOver.emplace(font, "GAME OVER", 48);

    tekstScore->setPosition({ 10.f, 55.f });
    tekstGameOver->setPosition({ 250.f, 250.f });
    tekstGameOver->setFillColor(sf::Color::Red);
}

void Gra::uruchom()
{
    sf::Clock zegar;

    while (okno.isOpen())
    {
        const float deltaCzas = zegar.restart().asSeconds();

        obslugaZdarzen();
        aktualizacja(deltaCzas);
        renderowanie();
    }
}

void Gra::obslugaZdarzen()
{
    while (const std::optional zdarzenie = okno.pollEvent())
    {
        if (zdarzenie->is<sf::Event::Closed>())
        {
            okno.close();
        }
    }
}

void Gra::aktualizacja(float deltaCzas)
{
    if (gameOver)
    {
        aktualizujUI();
        return;
    }

    aktualizujLiczniki(deltaCzas);
    sprawdzStartBossa();
    gracz->obslugaRuchu(deltaCzas);

    cooldownSpawnu = std::max(MIN_COOLDOWN_SPAWNU, START_COOLDOWN_SPAWNU - score / SKALOWANIE_COOLDOWNU);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && licznikStrzalu >= cooldownStrzalu)
    {
        strzel();
        licznikStrzalu = 0.f;
    }

    if (!boss && licznikSpawnu >= cooldownSpawnu)
    {
        spawnPrzeciwnika();
        licznikSpawnu = 0.f;
    }

    aktualizujObiekty(deltaCzas);
    strzalWrogow();
    strzalBossa();
    sprawdzKolizje();
    usunNieaktywneObiekty();
    aktualizujUI();
    sprawdzKoniecGry();
}

void Gra::aktualizujLiczniki(float deltaCzas)
{
    licznikStrzalu += deltaCzas;
    licznikSpawnu += deltaCzas;
    licznikKolizji += deltaCzas;
    licznikStrzaluWroga += deltaCzas;
    licznikStrzaluBossa += deltaCzas;
}

void Gra::aktualizujObiekty(float deltaCzas)
{
    for (auto& pocisk : pociski) pocisk.aktualizacja(deltaCzas);
    for (auto& enemy : przeciwnicy) enemy.aktualizacja(deltaCzas, gracz->pobierzPozycje());
    for (auto& ulepszenie : ulepszenia) ulepszenie.aktualizacja(deltaCzas);
    for (auto& pociskWroga : pociskiWrogow) pociskWroga.aktualizacja(deltaCzas);
    for (auto& eksplozja : eksplozje) eksplozja.aktualizacja(deltaCzas);

    if (boss)
    {
        boss->aktualizacja(deltaCzas);
    }
}

void Gra::sprawdzKoniecGry()
{
    if (gracz->pobierzZycia() > 0)
        return;

    gameOver = true;

    if (dzwiekKoncaOdtworzony)
        return;

    muzyka.stop();

    if (dzwiekKonca)
    {
        dzwiekKonca->play();
    }

    dzwiekKoncaOdtworzony = true;
}

void Gra::strzel()
{
    const float mnoznikPredkosci = szybkiePociski ? 2.f : 1.f;
    const sf::Vector2f pozycjaGracza = gracz->pobierzPozycje();

    if (podwojnyStrzal)
    {
        pociski.emplace_back(pozycjaGracza, 8.f, mnoznikPredkosci);
        pociski.emplace_back(pozycjaGracza, 32.f, mnoznikPredkosci);
    }
    else
    {
        pociski.emplace_back(pozycjaGracza, 20.f, mnoznikPredkosci);
    }

    if (dzwiekStrzalu)
    {
        dzwiekStrzalu->play();
    }
}

void Gra::sprawdzStartBossa()
{
    if (boss || score < nastepnyBossScore)
        return;

    przeciwnicy.clear();
    ulepszenia.clear();
    pociskiWrogow.clear();

    boss.emplace(zasoby.pobierzTeksture("boss"));
    licznikStrzaluBossa = 0.f;
}

void Gra::strzalBossa()
{
    if (!boss || licznikStrzaluBossa < cooldownStrzaluBossa)
        return;

    const sf::Vector2f srodekBossa = boss->pobierzSrodek();

    for (int i = 0; i < LICZBA_POCISKOW_BOSSA; ++i)
    {
        const float kat = (2.f * PI * i) / static_cast<float>(LICZBA_POCISKOW_BOSSA);
        pociskiWrogow.emplace_back(srodekBossa, sf::Vector2f{ std::cos(kat), std::sin(kat) }, 230.f);
    }

    licznikStrzaluBossa = 0.f;
}

void Gra::spawnPrzeciwnika()
{
    Enemy::Typ typ = Enemy::Typ::Meteoryt;
    sf::Texture* tekstura = &zasoby.pobierzTeksture("meteoryt");
    float maxX = 720.f;

    if (losujInt(0, 1) == 1)
    {
        typ = Enemy::Typ::Statek;
        tekstura = &zasoby.pobierzTeksture("wrog");
        maxX = 690.f;
    }

    przeciwnicy.emplace_back(
        sf::Vector2f(static_cast<float>(losujInt(0, static_cast<int>(maxX) - 1)), -120.f),
        typ,
        *tekstura
    );
}

void Gra::spawnUlepszenia(sf::Vector2f pozycja)
{
    if (losujInt(0, 9) != 0)
        return;

    static constexpr std::array typy = {
        TypUlepszenia::Szybkostrzelnosc,
        TypUlepszenia::Tarcza,
        TypUlepszenia::PodwojnyStrzal,
        TypUlepszenia::DodatkoweZycie
    };

    std::vector<TypUlepszenia> dostepne;

    for (const auto typ : typy)
    {
        if (czyUlepszenieDostepne(typ))
        {
            dostepne.push_back(typ);
        }
    }

    if (!dostepne.empty())
    {
        ulepszenia.emplace_back(pozycja, dostepne[losujInt(0, static_cast<int>(dostepne.size()) - 1)]);
    }
}

bool Gra::czyUlepszenieDostepne(TypUlepszenia typ) const
{
    switch (typ)
    {
    case TypUlepszenia::Szybkostrzelnosc: return !szybkiePociski;
    case TypUlepszenia::Tarcza: return !gracz->maTarcze();
    case TypUlepszenia::PodwojnyStrzal: return !podwojnyStrzal;
    case TypUlepszenia::DodatkoweZycie: return gracz->pobierzZycia() < gracz->pobierzMaxZycia();
    }

    return false;
}

void Gra::aktywujUlepszenie(TypUlepszenia typ)
{
    switch (typ)
    {
    case TypUlepszenia::Szybkostrzelnosc: szybkiePociski = true; break;
    case TypUlepszenia::Tarcza: gracz->wlaczTarcze(); break;
    case TypUlepszenia::PodwojnyStrzal: podwojnyStrzal = true; break;
    case TypUlepszenia::DodatkoweZycie: gracz->ulecz(1); break;
    }
}

void Gra::dodajEksplozje(sf::FloatRect granice)
{
    eksplozje.emplace_back(srodek(granice), klatkiEksplozji);

    if (dzwiekWybuchu)
    {
        dzwiekWybuchu->play();
    }
}

void Gra::strzalWrogow()
{
    if (licznikStrzaluWroga < cooldownStrzaluWroga)
        return;

    for (const auto& enemy : przeciwnicy)
    {
        if (!enemy.czyMartwy() && enemy.pobierzTyp() == Enemy::Typ::Statek)
        {
            pociskiWrogow.emplace_back(enemy.pobierzPozycjeStrzalu());
        }
    }

    licznikStrzaluWroga = 0.f;
}

void Gra::sprawdzKolizje()
{
    sprawdzKolizjePociskowZPrzeciwnikami();
    sprawdzKolizjePociskowZBossem();
    sprawdzKolizjeUlepszen();
    sprawdzKolizjePociskowWrogow();
    sprawdzKolizjeGraczaZPrzeciwnikami();
}

void Gra::sprawdzKolizjePociskowZPrzeciwnikami()
{
    for (auto& pocisk : pociski)
    {
        if (!pocisk.czyAktywny())
            continue;

        for (auto& enemy : przeciwnicy)
        {
            if (enemy.czyMartwy())
                continue;

            if (kolizja(pocisk.pobierzGranice(), enemy.pobierzGranice()))
            {
                enemy.otrzymajObrazenia(1);
                pocisk.dezaktywuj();
                break;
            }
        }
    }
}

void Gra::sprawdzKolizjePociskowZBossem()
{
    if (!boss)
        return;

    for (auto& pocisk : pociski)
    {
        if (!pocisk.czyAktywny())
            continue;

        if (kolizja(pocisk.pobierzGranice(), boss->pobierzGranice()))
        {
            boss->otrzymajObrazenia(1);
            pocisk.dezaktywuj();

            if (boss->czyMartwy())
            {
                dodajEksplozje(boss->pobierzGranice());
                boss.reset();
                score += 500;
                nastepnyBossScore += 1000;
            }

            break;
        }
    }
}

void Gra::sprawdzKolizjeUlepszen()
{
    for (auto& ulepszenie : ulepszenia)
    {
        if (ulepszenie.aktywne && kolizja(ulepszenie.pobierzGranice(), gracz->pobierzGranice()))
        {
            aktywujUlepszenie(ulepszenie.typ);
            ulepszenie.aktywne = false;
        }
    }
}

void Gra::sprawdzKolizjePociskowWrogow()
{
    for (auto& pociskWroga : pociskiWrogow)
    {
        if (pociskWroga.czyAktywny() && kolizja(pociskWroga.pobierzGranice(), gracz->pobierzGranice()))
        {
            gracz->otrzymajObrazenia(1);
            pociskWroga.dezaktywuj();
        }
    }
}

void Gra::sprawdzKolizjeGraczaZPrzeciwnikami()
{
    if (licznikKolizji < cooldownKolizji)
        return;

    for (const auto& enemy : przeciwnicy)
    {
        if (kolizja(gracz->pobierzGranice(), enemy.pobierzGranice()))
        {
            gracz->otrzymajObrazenia(1);
            licznikKolizji = 0.f;
            return;
        }
    }

    if (boss && kolizja(gracz->pobierzGranice(), boss->pobierzGranice()))
    {
        gracz->otrzymajObrazenia(1);
        licznikKolizji = 0.f;
    }
}

void Gra::usunNieaktywneObiekty()
{
    usunPociski();
    usunPociskiWrogow();
    usunUlepszenia();
    usunPrzeciwnikow();
    usunEksplozje();
}

void Gra::usunPociski()
{
    usunJesli(pociski, [](const Pocisk& pocisk)
    {
        return !pocisk.czyAktywny() || pocisk.pozaEkranem();
    });
}

void Gra::usunPociskiWrogow()
{
    usunJesli(pociskiWrogow, [](const PociskWroga& pocisk)
    {
        return !pocisk.czyAktywny() || pocisk.pozaEkranem();
    });
}

void Gra::usunUlepszenia()
{
    usunJesli(ulepszenia, [](const Ulepszenie& ulepszenie)
    {
        return !ulepszenie.aktywne || ulepszenie.pozaEkranem();
    });
}

void Gra::usunPrzeciwnikow()
{
    usunJesli(przeciwnicy, [this](const Enemy& enemy)
    {
        if (enemy.czyMartwy())
        {
            const sf::FloatRect granice = enemy.pobierzGranice();
            dodajEksplozje(granice);
            spawnUlepszenia(srodek(granice));
            score += 10;
            return true;
        }

        return enemy.pozaEkranem();
    });
}

void Gra::usunEksplozje()
{
    usunJesli(eksplozje, [](const Eksplozja& eksplozja)
    {
        return eksplozja.czyZakonczona();
    });
}

void Gra::aktualizujUI()
{
    tekstScore->setString("Score: " + std::to_string(score));
}

void Gra::renderowanie()
{
    okno.clear();
    okno.draw(*tlo);

    gracz->rysuj(okno);
    rysujPociski();

    if (boss)
    {
        boss->rysuj(okno);
        rysujPasekZyciaBossa();
    }

    rysujPrzeciwnikow();
    rysujEfekty();
    rysujUlepszenia();
    rysujUI();

    okno.display();
}

void Gra::rysujPociski()
{
    for (const auto& pocisk : pociski) pocisk.rysuj(okno);
    for (const auto& pociskWroga : pociskiWrogow) pociskWroga.rysuj(okno);
}

void Gra::rysujPrzeciwnikow()
{
    for (const auto& enemy : przeciwnicy) enemy.rysuj(okno);
}

void Gra::rysujEfekty()
{
    for (const auto& eksplozja : eksplozje) eksplozja.rysuj(okno);
}

void Gra::rysujUlepszenia()
{
    for (const auto& ulepszenie : ulepszenia) ulepszenie.rysuj(okno);
}

void Gra::rysujUI()
{
    const int liczbaZyc = gracz->pobierzZycia();

    for (int i = 0; i < liczbaZyc && i < static_cast<int>(serduszka.size()); ++i)
    {
        okno.draw(serduszka[i]);
    }

    okno.draw(*tekstScore);

    if (gameOver)
    {
        okno.draw(*tekstGameOver);
    }
}

void Gra::rysujPasekZyciaBossa()
{
    if (!boss)
        return;

    const sf::FloatRect granice = boss->pobierzGranice();
    const float szerokosc = granice.size.x;
    const float wysokosc = 12.f;
    const float x = granice.position.x;
    const float y = std::max(8.f, granice.position.y - 22.f);
    const float procent = std::max(0.f, static_cast<float>(boss->pobierzHp()) / static_cast<float>(boss->pobierzMaxHp()));

    sf::RectangleShape tloPaska({ szerokosc, wysokosc });
    tloPaska.setPosition({ x, y });
    tloPaska.setFillColor(sf::Color(40, 40, 40));
    tloPaska.setOutlineColor(sf::Color::White);
    tloPaska.setOutlineThickness(2.f);

    sf::RectangleShape pasek({ szerokosc * procent, wysokosc });
    pasek.setPosition({ x, y });
    pasek.setFillColor(sf::Color::Red);

    okno.draw(tloPaska);
    okno.draw(pasek);
}

int Gra::losujInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}