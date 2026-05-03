#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

#include "Biblioteca.h"
#include "Utilizator.h"
#include "CarteDigitala.h"
#include "Audiobook.h"
#include "MangaComics.h"
#include "FilmCarte.h"
#include "Database.h"

using namespace std;

// ═══════════════════════════════════════════════════════════
//  VARIABILE GLOBALE
// ═══════════════════════════════════════════════════════════

vector<Cont> conturi;

// ═══════════════════════════════════════════════════════════
//  UTILITARE UI
// ═══════════════════════════════════════════════════════════

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
        cout << "  Input invalid!\n";
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

// ═══════════════════════════════════════════════════════════
//  AUTENTIFICARE
// ═══════════════════════════════════════════════════════════

bool existaUsername(const string& username) {
    for (const auto& c : conturi)
        if (c.username == username) return true;
    return false;
}

Cont* loginCont(bool adminMode) {
    printHeader(adminMode ? "LOGIN ADMIN" : "LOGIN UTILIZATOR");
    string username = citesteString("Username: ");
    string parola   = citesteStringDirect("Parola: ");
    for (auto& c : conturi) {
        if (c.username == username && c.parola == parola) {
            if (adminMode && !c.esteAdmin) {
                cout << "\n  X Acest cont nu are drepturi de admin!\n";
                pauseScreen();
                return nullptr;
            }
            if (!adminMode && c.esteAdmin) {
                cout << "\n  X Foloseste Login Admin pentru contul de admin!\n";
                pauseScreen();
                return nullptr;
            }
            cout << "\n  OK Bun venit, " << username << "!\n";
            pauseScreen();
            return &c;
        }
    }
    cout << "\n  X Username sau parola incorecta!\n";
    pauseScreen();
    return nullptr;
}

Cont* creeazaCont(bool esteAdmin) {
    printHeader(esteAdmin ? "CREARE CONT ADMIN" : "CREARE CONT UTILIZATOR");
    string username = citesteString("Alege username: ");
    if (existaUsername(username)) {
        cout << "\n  X Username deja folosit!\n";
        pauseScreen();
        return nullptr;
    }
    string parola = citesteStringDirect("Alege parola: ");
    if (parola.empty()) {
        cout << "\n  X Parola nu poate fi goala!\n";
        pauseScreen();
        return nullptr;
    }
    conturi.push_back({username, parola, esteAdmin});
    salveazaConturi(conturi, FISIER_CONTURI);
    cout << "\n  OK Cont creat! Acum te poti loga.\n";
    pauseScreen();
    return &conturi.back();
}

// ═══════════════════════════════════════════════════════════
//  ADAUGARE CARTI (admin only)
// ═══════════════════════════════════════════════════════════

struct DateComune {
    string titlu, isbn, detalii, serie;
    vector<string> autori;
    int volum, parte, editieRevizie;
    double rating;
};

DateComune citesteDate() {
    DateComune d;
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

void adaugaCarteDigitala(Biblioteca& bib) {
    printHeader("ADAUGA CARTE DIGITALA");
    DateComune d = citesteDate();
    string format = citesteStringDirect("Format (PDF/EPUB/etc): ");
    bool drm = citesteBool("Are DRM");
    bib.adaugaCarte(make_unique<CarteDigitala>(
        d.titlu, d.autori, d.isbn, d.volum, d.parte,
        d.detalii, d.rating, d.editieRevizie, d.serie, format, drm));
    salveazaBiblioteca(bib, FISIER_BIBLIOTECA);
    cout << "\n  OK Carte digitala adaugata!\n";
    pauseScreen();
}

void adaugaAudiobook(Biblioteca& bib) {
    printHeader("ADAUGA AUDIOBOOK");
    DateComune d = citesteDate();
    string narator = citesteStringDirect("Narator: ");
    int durata = citesteInt("Durata (minute): ");
    bib.adaugaCarte(make_unique<Audiobook>(
        d.titlu, d.autori, d.isbn, d.volum, d.parte,
        d.detalii, d.rating, d.editieRevizie, d.serie, narator, durata));
    salveazaBiblioteca(bib, FISIER_BIBLIOTECA);
    cout << "\n  OK Audiobook adaugat!\n";
    pauseScreen();
}

void adaugaMangaComics(Biblioteca& bib) {
    printHeader("ADAUGA MANGA/COMICS");
    DateComune d = citesteDate();
    bool color = citesteBool("Este color");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string stil = citesteStringDirect("Stil (Manga/Marvel/DC): ");
    bib.adaugaCarte(make_unique<MangaComics>(
        d.titlu, d.autori, d.isbn, d.volum, d.parte,
        d.detalii, d.rating, d.editieRevizie, d.serie, color, stil));
    salveazaBiblioteca(bib, FISIER_BIBLIOTECA);
    cout << "\n  OK Manga adaugata!\n";
    pauseScreen();
}

void adaugaFilm(Biblioteca& bib) {
    printHeader("ADAUGA FILM");
    DateComune d = citesteDate();
    string regizor = citesteStringDirect("Regizor: ");
    int an = citesteInt("An lansare: ");
    bib.adaugaCarte(make_unique<FilmCarte>(
        d.titlu, d.autori, d.isbn, d.volum, d.parte,
        d.detalii, d.rating, d.editieRevizie, d.serie, regizor, an));
    salveazaBiblioteca(bib, FISIER_BIBLIOTECA);
    cout << "\n  OK Film adaugat!\n";
    pauseScreen();
}

void menuAdauga(Biblioteca& bib) {
    while (true) {
        printHeader("ADAUGA IN BIBLIOTECA");
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
            case 4: adaugaFilm(bib); break;
            case 0: return;
            default: cout << "  Optiune invalida!\n"; pauseScreen();
        }
    }
}

void menuSterge(Biblioteca& bib) {
    printHeader("STERGE DIN BIBLIOTECA");
    if (bib.numarCarti() == 0) {
        cout << "  Biblioteca este goala.\n";
        pauseScreen();
        return;
    }
    bib.afisareToate();
    cout << "\n";
    string titlu = citesteString("Titlu de sters: ");
    if (bib.stergeCarte(titlu)) {
        salveazaBiblioteca(bib, FISIER_BIBLIOTECA);
        cout << "\n  OK Cartea a fost stearsa.\n";
    } else {
        cout << "\n  X Cartea nu a fost gasita.\n";
    }
    pauseScreen();
}

// ═══════════════════════════════════════════════════════════
//  MENIU UTILIZATOR
// ═══════════════════════════════════════════════════════════

void menuUtilizator(Biblioteca& bib, Cont& contLogat) {
    Utilizator user(contLogat.username, 0);

    while (true) {
        printHeader("UTILIZATOR - " + contLogat.username);
        cout << "  1. Vezi biblioteca\n";
        cout << "  2. Cauta carte\n";
        cout << "  3. Imprumuta carte\n";
        cout << "  4. Returneaza carte\n";
        cout << "  5. Cartile mele\n";
        printSeparator();
        cout << "  0. Logout\n\n";

        int opt = citesteInt("Optiunea ta: ");

        if (opt == 0) {
            cout << "\n  La revedere, " << contLogat.username << "!\n";
            pauseScreen();
            return;
        } else if (opt == 1) {
            printHeader("BIBLIOTECA");
            cout << "  Total: " << bib.numarCarti() << " carti\n\n";
            bib.afisareToate();
            pauseScreen();
        } else if (opt == 2) {
            printHeader("CAUTA CARTE");
            string titlu = citesteString("Titlu cautat: ");
            cout << "\n";
            try { bib.cautaCarte(titlu); }
            catch (const exception& e) { cout << "  " << e.what() << "\n"; }
            pauseScreen();
        } else if (opt == 3) {
            printHeader("IMPRUMUTA CARTE");
            if (bib.numarCarti() == 0) {
                cout << "  Biblioteca este goala!\n";
                pauseScreen();
                continue;
            }
            bib.afisareToate();
            cout << "\n";
            string titlu = citesteString("Titlu de imprumutat: ");
            if (!bib.existaCarte(titlu))
                cout << "  X Cartea nu exista!\n";
            else {
                user.adaugaImprumut(titlu);
                cout << "  OK Carte imprumutata!\n";
            }
            pauseScreen();
        } else if (opt == 4) {
            printHeader("RETURNEAZA CARTE");
            string titlu = citesteString("Titlu de returnat: ");
            if (user.stergeImprumut(titlu))
                cout << "  OK Carte returnata!\n";
            else
                cout << "  X Nu ai aceasta carte imprumutata!\n";
            pauseScreen();
        } else if (opt == 5) {
            printHeader("CARTILE MELE");
            user.afisareInfo();
            pauseScreen();
        } else {
            cout << "  Optiune invalida!\n";
            pauseScreen();
        }
    }
}

// ═══════════════════════════════════════════════════════════
//  MENIU ADMIN
// ═══════════════════════════════════════════════════════════

void menuAdmin(Biblioteca& bib, Cont& contLogat) {
    while (true) {
        printHeader("ADMIN - " + contLogat.username);
        cout << "  1. Vezi biblioteca\n";
        cout << "  2. Cauta carte\n";
        cout << "  3. Adauga carte\n";
        cout << "  4. Sterge carte\n";
        cout << "  5. Lista conturi\n";
        printSeparator();
        cout << "  0. Logout\n\n";

        int opt = citesteInt("Optiunea ta: ");

        if (opt == 0) {
            cout << "\n  La revedere, Admin " << contLogat.username << "!\n";
            pauseScreen();
            return;
        } else if (opt == 1) {
            printHeader("BIBLIOTECA");
            cout << "  Total: " << bib.numarCarti() << " carti\n\n";
            bib.afisareToate();
            pauseScreen();
        } else if (opt == 2) {
            printHeader("CAUTA CARTE");
            string titlu = citesteString("Titlu cautat: ");
            cout << "\n";
            try { bib.cautaCarte(titlu); }
            catch (const exception& e) { cout << "  " << e.what() << "\n"; }
            pauseScreen();
        } else if (opt == 3) menuAdauga(bib);
        else if (opt == 4) menuSterge(bib);
        else if (opt == 5) {
            printHeader("LISTA CONTURI");
            for (int i = 0; i < (int)conturi.size(); i++)
                cout << "  " << i+1 << ". " << conturi[i].username
                     << (conturi[i].esteAdmin ? "  [ADMIN]" : "  [user]") << "\n";
            if (conturi.empty()) cout << "  Nu exista conturi.\n";
            pauseScreen();
        } else {
            cout << "  Optiune invalida!\n";
            pauseScreen();
        }
    }
}

// ═══════════════════════════════════════════════════════════
//  MAIN
// ═══════════════════════════════════════════════════════════

int main() {
    // Creeaza fisierele JSON default daca nu exista
    initializeazaFisiere();

    // Incarca conturi din conturi.json
    incarcaConturi(conturi, FISIER_CONTURI);

    // Incarca biblioteca din biblioteca.json
    Biblioteca biblioteca;
    incarcaBiblioteca(biblioteca, FISIER_BIBLIOTECA);

    while (true) {
        printHeader("ECRAN PRINCIPAL");
        cout << "  1. Login Utilizator\n";
        cout << "  2. Creeaza cont Utilizator\n";
        printSeparator();
        cout << "  3. Login Admin\n";
        cout << "  4. Creeaza cont Admin\n";
        printSeparator();
        cout << "  0. Iesire\n\n";

        int opt = citesteInt("Optiunea ta: ");

        if (opt == 0) {
            salveazaBiblioteca(biblioteca, FISIER_BIBLIOTECA);
            clearScreen();
            cout << "  La revedere!\n\n";
            return 0;
        } else if (opt == 1) {
            Cont* c = loginCont(false);
            if (c) menuUtilizator(biblioteca, *c);
        } else if (opt == 2) {
            creeazaCont(false);
        } else if (opt == 3) {
            Cont* c = loginCont(true);
            if (c) menuAdmin(biblioteca, *c);
        } else if (opt == 4) {
            creeazaCont(true);
        } else {
            cout << "  Optiune invalida!\n";
            pauseScreen();
        }
    }
}
