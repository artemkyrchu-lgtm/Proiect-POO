#ifndef UTILIZATOR_H
#define UTILIZATOR_H
using namespace std;
#include <string>
#include <vector>
#include "Carte.h"

class Utilizator {
private:
    std::string nume;
    int id;
    std::vector<std::string> imprumuturi;

public:
    Utilizator(std::string n, int i);
    void adaugaImprumut(std::string titlu);
    void afisareInfo() const;
};

#endif
