#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include "Biblioteca.h"

// ── Fisiere implicite ─────────────────────────────────────────────────────────
const std::string FISIER_BIBLIOTECA = "biblioteca.json";
const std::string FISIER_CONTURI    = "conturi.json";

// ── Structura Cont (cu campuri extinse) ───────────────────────────────────────
struct Cont {
    std::string username;
    std::string parola;
    bool esteAdmin = false;
    std::string nume;
    std::string prenume;
    std::string dataNasterii;             // format: DD.MM.YYYY
    std::vector<std::string> imprumuturi; // lista titluri imprumutate (doar useri)
};

// ── Biblioteca JSON ───────────────────────────────────────────────────────────
bool salveazaBiblioteca(const Biblioteca& bib,
                        const std::string& numeFisier = "biblioteca.json");

bool incarcaBiblioteca(Biblioteca& bib,
                       const std::string& numeFisier = "biblioteca.json");

// ── Conturi JSON ──────────────────────────────────────────────────────────────
bool salveazaConturi(const std::vector<Cont>& conturi,
                     const std::string& numeFisier = "conturi.json");

bool incarcaConturi(std::vector<Cont>& conturi,
                    const std::string& numeFisier = "conturi.json");

// Creeaza fisierele JSON cu date default daca nu exista inca
void initializeazaFisiere();

#endif
