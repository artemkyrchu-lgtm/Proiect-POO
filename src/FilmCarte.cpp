#include "FilmCarte.h"

FilmCarte::FilmCarte(std::string t, std::vector<std::string> aut, std::string i, int v, int p,
                     std::string det, double rat, int rev, std::string ser,
                     std::string reg, int an)
    : Carte(t, aut, i, v, p, det, rat, rev, ser), regizor(reg), anLansare(an) {}

void FilmCarte::afisareDetalii() const {
    std::cout << "[FILM] " << titlu
              << " | Regizor: " << regizor
              << " | An: " << anLansare
              << " | Rating: " << rating
              << std::endl;
}
