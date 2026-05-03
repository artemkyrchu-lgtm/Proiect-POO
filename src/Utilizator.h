#ifndef UTILIZATOR_H
#define UTILIZATOR_H

#include <string>
#include <vector>
#include <iostream>

class Utilizator {
private:
    std::string nume;
    int id;
    std::vector<std::string> imprumuturi;

public:
    Utilizator(std::string n, int i);
    void adaugaImprumut(const std::string& titlu);
    bool stergeImprumut(const std::string& titlu);
    void afisareInfo() const;
    std::string getNume() const { return nume; }
    int getId() const { return id; }
    const std::vector<std::string>& getImprumuturi() const { return imprumuturi; }
};

#endif
