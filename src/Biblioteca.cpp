#include "Biblioteca.h"
#include <iostream>
#include <stdexcept>
using namespace std;
void Biblioteca::adaugaCarte(std::unique_ptr<Carte> c) {
    catalog.push_back(std::move(c));
}

void Biblioteca::afisareToate() const {
    if (catalog.empty()) {
        std::cout << "Biblioteca este goala.\n";
        return;
    }
    for (const auto& c : catalog) {
        c->afisareDetalii();
    }
}

void Biblioteca::cautaCarte(std::string titlu) const {
    for (const auto& c : catalog) {
        if (c->getTitlu() == titlu) {
            std::cout << "Carte gasita!\n";
            return;
        }
    }

    throw std::runtime_error("Eroare: Carte indisponibila!");
}
