#ifndef CARTE_H
#define CARTE_H

#include <string>
#include <iostream>
using namespace std;

class Carte {
protected:
    std::string titlu;
    std::string autor;
    std::string isbn;

public:
    Carte(std::string t, std::string a, std::string i);
    virtual ~Carte() {}


    virtual void afisareDetalii() const;

    std::string getTitlu() const { return titlu; }
};


class CarteFictiune : public Carte {
public:
    CarteFictiune(std::string t, std::string a, std::string i) : Carte(t, a, i) {}
    void afisareDetalii() const override;
};

class CarteTehnica : public Carte {
public:
    CarteTehnica(std::string t, std::string a, std::string i) : Carte(t, a, i) {}
    void afisareDetalii() const override;
};

#endif
