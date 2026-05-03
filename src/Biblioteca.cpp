#include "Biblioteca.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

void Biblioteca::adaugaCarte(std::unique_ptr<Carte> c) {
    catalog.push_back(std::move(c));
}

bool Biblioteca::stergeCarte(const std::string& titlu) {
    auto it = std::find_if(catalog.begin(), catalog.end(),
        [&titlu](const std::unique_ptr<Carte>& c) {
            return c->getTitlu() == titlu;
        });
    if (it != catalog.end()) {
        catalog.erase(it);
        return true;
    }
    return false;
}

void Biblioteca::afisareToate() const {
    if (catalog.empty()) {
        std::cout << "  Biblioteca este goala.\n";
        return;
    }
    int i = 1;
    for (const auto& c : catalog) {
        std::cout << "  " << i++ << ". ";
        c->afisareDetalii();
    }
}

void Biblioteca::cautaCarte(const std::string& titlu) const {
    for (const auto& c : catalog) {
        if (c->getTitlu() == titlu) {
            std::cout << "  Carte gasita:\n  ";
            c->afisareDetalii();
            return;
        }
    }
    throw std::runtime_error("Eroare: Cartea \"" + titlu + "\" nu a fost gasita!");
}

bool Biblioteca::existaCarte(const std::string& titlu) const {
    for (const auto& c : catalog) {
        if (c->getTitlu() == titlu) return true;
    }
    return false;
}
