#include "MangaComics.h"

MangaComics::MangaComics(std::string t, std::vector<std::string> aut, std::string i, int v, int p,
                         std::string det, double rat, int rev, std::string ser,
                         bool color, std::string stil)
    : Carte(t, aut, i, v, p, det, rat, rev, ser), esteColor(color), stilDesen(stil) {}

void MangaComics::afisareDetalii() const {
    std::cout << "[MANGA/COMICS] " << titlu
              << " | Stil: " << stilDesen
              << " | Color: " << (esteColor ? "Da" : "Nu")
              << " | Rating: " << rating
              << std::endl;
}
