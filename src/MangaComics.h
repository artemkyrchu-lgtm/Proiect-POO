#ifndef MANGA_COMICS_H
#define MANGA_COMICS_H

#include "Carte.h"

class MangaComics : public Carte {
private:
    bool esteColor;
    std::string stilDesen;

public:
    MangaComics(std::string t, std::vector<std::string> aut, std::string i, int v, int p,
                std::string det, double rat, int rev, std::string ser,
                bool color, std::string stil);

    void afisareDetalii() const override;
    std::string getTip() const override { return "MangaComics"; }
    std::string getExtra1() const override { return stilDesen; }
    bool getExtraBool() const override { return esteColor; }
};

#endif
