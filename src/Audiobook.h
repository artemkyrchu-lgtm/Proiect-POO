#ifndef AUDIOBOOK_H
#define AUDIOBOOK_H

#include "Carte.h"

class Audiobook : public Carte {
private:
    std::string narator;
    int durataMinute;

public:
    Audiobook(std::string t, std::vector<std::string> aut, std::string i, int v, int p,
              std::string det, double rat, int rev, std::string ser,
              std::string nar, int durata);

    void afisareDetalii() const override;
    std::string getTip() const override { return "Audiobook"; }
};

#endif
