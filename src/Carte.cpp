#include "Carte.h"
using namespace std;
Carte::Carte(std::string t, std::string a, std::string i) : titlu(t), autor(a), isbn(i) {}

void Carte::afisareDetalii() const {
    std::cout << "[CARTE] " << titlu << " | Autor: " << autor << " | ISBN: " << isbn << std::endl;
}

void CarteFictiune::afisareDetalii() const {
    std::cout << "[FICTIUNE] " << titlu << " - O poveste captivanta." << std::endl;
}

void CarteTehnica::afisareDetalii() const {
    std::cout << "[TEHNICA] " << titlu << " - Manual de specialitate." << std::endl;
}
