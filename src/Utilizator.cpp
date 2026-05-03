#include "Utilizator.h"
#include <algorithm>

Utilizator::Utilizator(std::string n, int i) : nume(n), id(i) {}

void Utilizator::adaugaImprumut(const std::string& titlu) {
    imprumuturi.push_back(titlu);
}

bool Utilizator::stergeImprumut(const std::string& titlu) {
    auto it = std::find(imprumuturi.begin(), imprumuturi.end(), titlu);
    if (it != imprumuturi.end()) {
        imprumuturi.erase(it);
        return true;
    }
    return false;
}

void Utilizator::afisareInfo() const {
    std::cout << "Utilizator: " << nume << " (ID: " << id << ")\n";
    std::cout << "Carti imprumutate: ";
    if (imprumuturi.empty()) {
        std::cout << "Niciuna";
    } else {
        for (const auto& c : imprumuturi)
            std::cout << "\n  - " << c;
    }
    std::cout << "\n";
}
