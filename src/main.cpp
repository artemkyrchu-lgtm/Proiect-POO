#include <iostream>
#include "Biblioteca.h"
#include "Utilizator.h"
using namespace std;
int main() {
    Biblioteca bib;
    Utilizator user("Student", 2024);
    int optiune;

    do {
        std::cout << "\n--- SISTEM BIBLIOTECA ---\n";
        std::cout << "1. Adauga Carte Fictiune\n";
        std::cout << "2. Adauga Carte Tehnica\n";
        std::cout << "3. Afisare Catalog\n";
        std::cout << "4. Cauta Carte (Excepții)\n";
        std::cout << "0. Iesire\n";
        std::cout << "Selectati: ";
        std::cin >> optiune;

        try {
            if (optiune == 1 || optiune == 2) {
                std::string t, a, i;
                std::cout << "Titlu: "; std::cin.ignore(); std::getline(std::cin, t);
                std::cout << "Autor: "; std::getline(std::cin, a);
                std::cout << "ISBN: "; std::getline(std::cin, i);

                if (optiune == 1)
                    bib.adaugaCarte(std::make_unique<CarteFictiune>(t, a, i));
                else
                    bib.adaugaCarte(std::make_unique<CarteTehnica>(t, a, i));
            }
            else if (optiune == 3) {
                bib.afisareToate();
            }
            else if (optiune == 4) {
                std::string t;
                std::cout << "Introduceti titlul: "; std::cin.ignore(); std::getline(std::cin, t);
                bib.cautaCarte(t);
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    } while (optiune != 0);

    return 0;
}
