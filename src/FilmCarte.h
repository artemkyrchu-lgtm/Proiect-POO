#ifndef FILM_CARTE_H
#define FILM_CARTE_H

#include "Carte.h"

class FilmCarte : public Carte {
private:
    std::string regizor;
    int anLansare;

public:
    FilmCarte(std::string t, std::vector<std::string> aut, std::string i, int v, int p,
              std::string det, double rat, int rev, std::string ser,
              std::string reg, int an);

    void afisareDetalii() const override;
    std::string getTip() const override { return "FilmCarte"; }
    std::string getExtra1() const override { return regizor; }
    int getExtraInt() const override { return anLansare; }
};

#endif
