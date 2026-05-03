#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <vector>
#include <memory>
#include <string>
#include "Carte.h"

class Biblioteca {
private:
    std::vector<std::unique_ptr<Carte>> catalog;

public:
    void adaugaCarte(std::unique_ptr<Carte> c);
    bool stergeCarte(const std::string& titlu);
    void afisareToate() const;
    void cautaCarte(const std::string& titlu) const;
    bool existaCarte(const std::string& titlu) const;
    int numarCarti() const { return (int)catalog.size(); }
    const std::vector<std::unique_ptr<Carte>>& getCatalog() const { return catalog; }
};

#endif
