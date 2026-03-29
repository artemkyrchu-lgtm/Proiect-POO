#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H
using namespace std;
#include <vector>
#include <memory>
#include "Carte.h"

class Biblioteca {
private:

    std::vector<std::unique_ptr<Carte>> catalog;

public:
    void adaugaCarte(std::unique_ptr<Carte> c);
    void afisareToate() const;

    void cautaCarte(std::string titlu) const;
};

#endif
