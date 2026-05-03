#include "Audiobook.h"

Audiobook::Audiobook(std::string t, std::vector<std::string> aut, std::string i, int v, int p,
                     std::string det, double rat, int rev, std::string ser,
                     std::string nar, int durata)
    : Carte(t, aut, i, v, p, det, rat, rev, ser), narator(nar), durataMinute(durata) {}

void Audiobook::afisareDetalii() const {
    std::cout << "[AUDIOBOOK] " << titlu
              << " | Narator: " << narator
              << " | Durata: " << durataMinute << " min"
              << " | Rating: " << rating
              << std::endl;
}
