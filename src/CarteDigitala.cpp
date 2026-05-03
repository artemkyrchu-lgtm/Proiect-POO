#include "CarteDigitala.h"

CarteDigitala::CarteDigitala(std::string t, std::vector<std::string> aut, std::string i, int v, int p,
                             std::string det, double rat, int rev, std::string ser,
                             std::string format, bool drm)
    : Carte(t, aut, i, v, p, det, rat, rev, ser), formatFisier(format), areDrm(drm) {}

void CarteDigitala::afisareDetalii() const {
    std::cout << "[CARTE DIGITALA] " << titlu
              << " | Format: " << formatFisier
              << " | DRM: " << (areDrm ? "Da" : "Nu")
              << " | ISBN: " << isbn
              << " | Rating: " << rating
              << std::endl;
}
