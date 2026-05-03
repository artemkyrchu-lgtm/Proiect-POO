#ifndef CARTE_DIGITALA_H
#define CARTE_DIGITALA_H

#include "Carte.h"

class CarteDigitala : public Carte {
private:
    std::string formatFisier;
    bool areDrm;

public:
    CarteDigitala(std::string t, std::vector<std::string> aut, std::string i, int v, int p,
                  std::string det, double rat, int rev, std::string ser,
                  std::string format, bool drm);

    void afisareDetalii() const override;
    std::string getTip() const override { return "Carte Digitala"; }
};

#endif
