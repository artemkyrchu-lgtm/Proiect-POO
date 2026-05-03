#ifndef CARTE_H
#define CARTE_H

#include <string>
#include <iostream>
#include <vector>

class Carte {
protected:
    std::string titlu;
    std::vector<std::string> autori;
    std::string isbn;
    int volum;
    int parte;
    std::string detalii;
    double rating;
    int editieRevizie;
    std::string serie;

public:
    Carte(std::string t, std::vector<std::string> aut, std::string i, int v, int p,
          std::string det, double rat, int rev, std::string ser);

    virtual ~Carte() {}

    virtual void afisareDetalii() const = 0;
    virtual std::string getTip() const = 0;

    std::string getTitlu() const { return titlu; }
    std::string getIsbn() const { return isbn; }
    std::string getSerie() const { return serie; }
    double getRating() const { return rating; }
    int getVolum() const { return volum; }
    int getParte() const { return parte; }
    std::string getDetalii() const { return detalii; }
    int getEditieRevizie() const { return editieRevizie; }
    std::vector<std::string> getAutori() const { return autori; }
    virtual std::string getExtra1() const { return ""; }
    virtual std::string getExtra2() const { return ""; }
    virtual int getExtraInt() const { return 0; }
    virtual bool getExtraBool() const { return false; }
};

#endif
