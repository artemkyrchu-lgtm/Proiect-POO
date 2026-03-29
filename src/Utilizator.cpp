#include "Utilizator.h"
#include <iostream>
using namespace std;
Utilizator::Utilizator(std::string n, int i) : nume(n), id(i) {}

void Utilizator::adaugaImprumut(std::string titlu) {
    imprumuturi.push_back(titlu);
}

void Utilizator::afisareInfo() const {
    std::cout << "Utilizator: " << nume << " (ID: " << id << ")\n";
    std::cout << "Carti imprumutate: ";
    if (imprumuturi.empty()) std::cout << "Niciuna";
    for (const auto& c : imprumuturi) std::cout << c << " ";
    std::cout << "\n";
}
