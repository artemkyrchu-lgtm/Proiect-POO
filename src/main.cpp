#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <limits>

#include "Biblioteca.h"
#include "Utilizator.h"
#include "CarteDigitala.h"
#include "Audiobook.h"
#include "MangaComics.h"
#include "FilmCarte.h"

using namespace std;

// ─── Utilitare ────────────────────────────────────────────────────────────────

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\n  Apasa Enter pentru a continua...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printSeparator(char c = '-', int n = 50) {
    cout << "  ";
    for (int i = 0; i < n; i++) cout << c;
    cout << "\n";
}

void printHeader(const string& titlu) {
    clearScreen();
    printSeparator('=');
    cout << "  SISTEM BIBLIOTECA - " << titlu << "\n";
    printSeparator('=');
    cout << "\n";
}

string citesteString(const string& prompt) {
    string val;
    cout << "  " << prompt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, val);
    return val;
}

string citesteStringDirect(const string& prompt) {
    string val;
    cout << "  " << prompt;
    getline(cin, val);
    return val;
}

int citesteInt(const string& prompt) {
    int val;
    while (true) {
        cout << "  " << prompt;
        if (cin >> val) break;
        cout << "  Input invalid! Introdu un numar.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return val;
}

double citesteDouble(const string& prompt) {
    double val;
    while (true) {
        cout << "  " << prompt;
        if (cin >> val) break;
        cout << "  Input invalid!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return val;
}

bool citesteBool(const string& prompt) {
    int val;
    while (true) {
        cout << "  " << prompt << " (1=Da, 0=Nu): ";
        if (cin >> val && (val == 0 || val == 1)) return (bool)val;
        cout << "  Input invalid! Introdu 1 sau 0.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

vector<string> citesteAutori() {
    vector<string> autori;
    int n = citesteInt("Numar de autori: ");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < n; i++) {
        string autor;
        cout << "  Autor " << (i + 1) << ": ";
        getline(cin, autor);
        autori.push_back(autor);
    }
    return autori;
}

// ─── Campuri comune pentru orice carte ───────────────────────────────────────

struct DateComune {
    string titlu, isbn, detalii, serie;
    vector<string> autori;
    int volum, parte, editieRevizie;
    double rating;
};

DateComune citesteDate() {
    DateComune d;
    // citesteString face cin.ignore inainte de getline - fix pentru buffer '\n'
    d.titlu         = citesteString("Titlu: ");
    d.autori        = citesteAutori();
    d.isbn          = citesteStringDirect("ISBN: ");
    d.volum         = citesteInt("Volum: ");
    d.parte         = citesteInt("Parte: ");
    d.editieRevizie = citesteInt("Editie/Revizie: ");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    d.detalii       = citesteStringDirect("Detalii: ");
    d.rating        = citesteDouble("Rating (0-10): ");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    d.serie         = citesteStringDirect("Serie: ");
    return d;
}

// ─── Adaugare carti ──────────────────────────────────────────────────────────

void adaugaCarteDigitala(Biblioteca& bib) {
    printHeader("ADAUGA CARTE DIGITALA");
    DateComune d = citesteDate();
    string format = citesteStringDirect("Format fisier (PDF/EPUB/etc): ");
    bool drm = citesteBool("Are DRM");

    bib.adaugaCarte(make_unique<CarteDigitala>(
        d.titlu, d.autori, d.isbn, d.volum, d.parte,
        d.detalii, d.rating, d.editieRevizie, d.serie,
        format, drm));

    cout << "\n  ✓ Carte digitala adaugata cu succes!\n";
    pauseScreen();
}

void adaugaAudiobook(Biblioteca& bib) {
    printHeader("ADAUGA AUDIOBOOK");
    DateComune d = citesteDate();
    string narator = citesteStringDirect("Narator: ");
    int durata = citesteInt("Durata (minute): ");

    bib.adaugaCarte(make_unique<Audiobook>(
        d.titlu, d.autori, d.isbn, d.volum, d.parte,
        d.detalii, d.rating, d.editieRevizie, d.serie,
        narator, durata));

    cout << "\n  ✓ Audiobook adaugat cu succes!\n";
    pauseScreen();
}

void adaugaMangaComics(Biblioteca& bib) {
    printHeader("ADAUGA MANGA/COMICS");
    DateComune d = citesteDate();
    bool color = citesteBool("Este color");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string stil = citesteStringDirect("Stil desen (Manga/Marvel/DC/etc): ");

    bib.adaugaCarte(make_unique<MangaComics>(
        d.titlu, d.autori, d.isbn, d.volum, d.parte,
        d.detalii, d.rating, d.editieRevizie, d.serie,
        color, stil));

    cout << "\n  ✓ Manga/Comics adaugat cu succes!\n";
    pauseScreen();
}

void adaugaFilmCarte(Biblioteca& bib) {
    printHeader("ADAUGA FILM");
    DateComune d = citesteDate();
    string regizor = citesteStringDirect("Regizor: ");
    int an = citesteInt("An lansare: ");

    bib.adaugaCarte(make_unique<FilmCarte>(
        d.titlu, d.autori, d.isbn, d.volum, d.parte,
        d.detalii, d.rating, d.editieRevizie, d.serie,
        regizor, an));

    cout << "\n  ✓ Film adaugat cu succes!\n";
    pauseScreen();
}

void menuAdauga(Biblioteca& bib) {
    while (true) {
        printHeader("ADAUGA IN BIBLIOTECA");
        cout << "  Alege tipul:\n\n";
        cout << "  1. Carte Digitala\n";
        cout << "  2. Audiobook\n";
        cout << "  3. Manga / Comics\n";
        cout << "  4. Film\n";
        cout << "  0. Inapoi\n\n";

        int opt = citesteInt("Optiunea ta: ");
        switch (opt) {
            case 1: adaugaCarteDigitala(bib); break;
            case 2: adaugaAudiobook(bib); break;
            case 3: adaugaMangaComics(bib); break;
            case 4: adaugaFilmCarte(bib); break;
            case 0: return;
            default: cout << "  Optiune invalida!\n"; pauseScreen();
        }
    }
}

// ─── Stergere ─────────────────────────────────────────────────────────────────

void menuSterge(Biblioteca& bib) {
    printHeader("STERGE DIN BIBLIOTECA");

    if (bib.numarCarti() == 0) {
        cout << "  Biblioteca este goala, nu ai ce sterge.\n";
        pauseScreen();
        return;
    }

    cout << "  Carti disponibile:\n\n";
    bib.afisareToate();
    cout << "\n";

    string titlu = citesteString("Titlu de sters: ");
    if (bib.stergeCarte(titlu)) {
        cout << "\n  ✓ Cartea \"" << titlu << "\" a fost stearsa.\n";
    } else {
        cout << "\n  ✗ Cartea \"" << titlu << "\" nu a fost gasita.\n";
    }
    pauseScreen();
}

// ─── Vizualizare ──────────────────────────────────────────────────────────────

void menuVizualizeaza(Biblioteca& bib) {
    printHeader("BIBLIOTECA - TOATE CARTILE");
    cout << "  Total: " << bib.numarCarti() << " inregistrari\n\n";
    bib.afisareToate();
    pauseScreen();
}

void menuCauta(Biblioteca& bib) {
    printHeader("CAUTA CARTE");
    string titlu = citesteString("Titlu cautat: ");
    cout << "\n";
    try {
        bib.cautaCarte(titlu);
    } catch (const exception& e) {
        cout << "  " << e.what() << "\n";
    }
    pauseScreen();
}

// ─── Utilizator ───────────────────────────────────────────────────────────────

void menuUtilizator(Biblioteca& bib) {
    static vector<Utilizator> utilizatori;

    while (true) {
        printHeader("GESTIONARE UTILIZATORI");
        cout << "  1. Adauga utilizator\n";
        cout << "  2. Imprumuta carte\n";
        cout << "  3. Returneaza carte\n";
        cout << "  4. Afiseaza utilizatori\n";
        cout << "  0. Inapoi\n\n";

        int opt = citesteInt("Optiunea ta: ");

        if (opt == 0) return;

        if (opt == 1) {
            printHeader("ADAUGA UTILIZATOR");
            string nume = citesteString("Nume utilizator: ");
            int id = citesteInt("ID utilizator: ");
            utilizatori.emplace_back(nume, id);
            cout << "\n  ✓ Utilizator adaugat!\n";
            pauseScreen();
        }
        else if (opt == 2) {
            printHeader("IMPRUMUTA CARTE");
            if (utilizatori.empty()) { cout << "  Nu exista utilizatori!\n"; pauseScreen(); continue; }
            for (int i = 0; i < (int)utilizatori.size(); i++)
                cout << "  " << i+1 << ". " << utilizatori[i].getNume() << " (ID:" << utilizatori[i].getId() << ")\n";
            int idx = citesteInt("Alege utilizator (nr): ") - 1;
            if (idx < 0 || idx >= (int)utilizatori.size()) { cout << "  Index invalid!\n"; pauseScreen(); continue; }
            string titlu = citesteString("Titlu carte de imprumutat: ");
            if (!bib.existaCarte(titlu)) { cout << "  ✗ Cartea nu exista in biblioteca!\n"; pauseScreen(); continue; }
            utilizatori[idx].adaugaImprumut(titlu);
            cout << "  ✓ Carte imprumutata!\n";
            pauseScreen();
        }
        else if (opt == 3) {
            printHeader("RETURNEAZA CARTE");
            if (utilizatori.empty()) { cout << "  Nu exista utilizatori!\n"; pauseScreen(); continue; }
            for (int i = 0; i < (int)utilizatori.size(); i++)
                cout << "  " << i+1 << ". " << utilizatori[i].getNume() << "\n";
            int idx = citesteInt("Alege utilizator (nr): ") - 1;
            if (idx < 0 || idx >= (int)utilizatori.size()) { cout << "  Index invalid!\n"; pauseScreen(); continue; }
            string titlu = citesteString("Titlu carte de returnat: ");
            if (utilizatori[idx].stergeImprumut(titlu))
                cout << "  ✓ Carte returnata!\n";
            else
                cout << "  ✗ Cartea nu e la acest utilizator!\n";
            pauseScreen();
        }
        else if (opt == 4) {
            printHeader("UTILIZATORI");
            if (utilizatori.empty()) { cout << "  Nu exista utilizatori.\n"; }
            for (auto& u : utilizatori) { u.afisareInfo(); cout << "\n"; }
            pauseScreen();
        }
        else {
            cout << "  Optiune invalida!\n";
            pauseScreen();
        }
    }
}

// ─── Meniu Principal ──────────────────────────────────────────────────────────

int main() {
    Biblioteca biblioteca;

    // Date demo
    biblioteca.adaugaCarte(make_unique<CarteDigitala>(
        "Programare in C++", vector<string>{"Bjarne Stroustrup"},
        "ISBN-001", 1, 1, "Manual avansat", 9.5, 3, "SER-001", "PDF", true));
    biblioteca.adaugaCarte(make_unique<Audiobook>(
        "Micul Print", vector<string>{"Antoine de Saint-Exupery"},
        "ISBN-002", 1, 1, "Fictiune clasica", 9.8, 1, "SER-002",
        "Victor Rebengiuc", 180));

    while (true) {
        printHeader("MENIU PRINCIPAL");
        cout << "  1. Vizualizeaza biblioteca\n";
        cout << "  2. Cauta o carte\n";
        cout << "  3. Adauga in biblioteca\n";
        cout << "  4. Sterge din biblioteca\n";
        cout << "  5. Gestionare utilizatori\n";
        printSeparator();
        cout << "  0. Iesire\n\n";

        int opt = citesteInt("Optiunea ta: ");

        switch (opt) {
            case 1: menuVizualizeaza(biblioteca); break;
            case 2: menuCauta(biblioteca); break;
            case 3: menuAdauga(biblioteca); break;
            case 4: menuSterge(biblioteca); break;
            case 5: menuUtilizator(biblioteca); break;
            case 0:
                clearScreen();
                cout << "  La revedere!\n\n";
                return 0;
            default:
                cout << "  Optiune invalida!\n";
                pauseScreen();
        }
    }
}
