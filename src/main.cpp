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
//  AUTENTIFICARE SI CONTURI
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
            cout << "\n  OK Bun venit, " << c.prenume << " " << c.nume << "!\n";
            pauseScreen();
            return &c;
        }
    }
    cout << "\n  X Username sau parola incorecta!\n";
    pauseScreen();
    return nullptr;
}

// citeste campurile comune (nume, prenume, data nasterii) pentru orice cont
void citesteDatePersonale(Cont& c) {
    c.nume         = citesteStringDirect("Nume: ");
    c.prenume      = citesteStringDirect("Prenume: ");
    c.dataNasterii = citesteStringDirect("Data nasterii (DD.MM.YYYY): ");
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
    Cont c;
    c.username  = username;
    c.parola    = parola;
    c.esteAdmin = esteAdmin;
    citesteDatePersonale(c);

    conturi.push_back(c);
    salveazaConturi(conturi, FISIER_CONTURI);
    cout << "\n  OK Cont creat! Acum te poti loga.\n";
    pauseScreen();
    return &conturi.back();
}

// ═══════════════════════════════════════════════════════════
//  CAUTARE DUPA CATEGORIE
// ═══════════════════════════════════════════════════════════

void cautaDupaTip(const Biblioteca& bib) {
    printHeader("CAUTA DUPA CATEGORIE");
    cout << "  1. Carte Digitala\n";
    cout << "  2. Audiobook\n";
    cout << "  3. Manga / Comics\n";
    cout << "  4. Film\n";
    cout << "  0. Inapoi\n\n";
    int opt = citesteInt("Categorie: ");
    if (opt == 0) return;

    string tipCautat;
    string eticheta;
    switch (opt) {
        case 1: tipCautat = "CarteDigitala"; eticheta = "CARTI DIGITALE"; break;
        case 2: tipCautat = "Audiobook";     eticheta = "AUDIOBOOK-URI";  break;
        case 3: tipCautat = "MangaComics";   eticheta = "MANGA / COMICS"; break;
        case 4: tipCautat = "FilmCarte";     eticheta = "FILME";          break;
        default:
            cout << "  Optiune invalida!\n";
            pauseScreen();
            return;
    }

    printHeader(eticheta);
    int nr = 0;
    for (const auto& c : bib.getCatalog()) {
        if (c->getTip() == tipCautat) {
            cout << "  " << ++nr << ". ";
            c->afisareDetalii();
        }
    }
    if (nr == 0) cout << "  Nu exista intrari in aceasta categorie.\n";
    else cout << "\n  Total: " << nr << " intrari.\n";
    pauseScreen();
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
    while (true) {
        printHeader("UTILIZATOR - " + contLogat.prenume + " " + contLogat.nume);
        cout << "  1. Vezi biblioteca\n";
        cout << "  2. Cauta carte dupa titlu\n";
        cout << "  3. Cauta dupa categorie\n";
        cout << "  4. Imprumuta carte\n";
        cout << "  5. Returneaza carte\n";
        cout << "  6. Cartile mele\n";
        printSeparator();
        cout << "  0. Logout\n\n";

        int opt = citesteInt("Optiunea ta: ");

        if (opt == 0) {
            cout << "\n  La revedere, " << contLogat.prenume << "!\n";
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
            cautaDupaTip(bib);
        } else if (opt == 4) {
            printHeader("IMPRUMUTA CARTE");
            if (bib.numarCarti() == 0) {
                cout << "  Biblioteca este goala!\n";
                pauseScreen();
                continue;
            }
            bib.afisareToate();
            cout << "\n";
            string titlu = citesteString("Titlu de imprumutat: ");
            if (!bib.existaCarte(titlu)) {
                cout << "  X Cartea nu exista!\n";
            } else {
                // verifica daca nu are deja imprumutata
                auto it = find(contLogat.imprumuturi.begin(),
                               contLogat.imprumuturi.end(), titlu);
                if (it != contLogat.imprumuturi.end()) {
                    cout << "  X Ai deja aceasta carte imprumutata!\n";
                } else {
                    contLogat.imprumuturi.push_back(titlu);
                    salveazaConturi(conturi, FISIER_CONTURI);
                    cout << "  OK Carte imprumutata!\n";
                }
            }
            pauseScreen();
        } else if (opt == 5) {
            printHeader("RETURNEAZA CARTE");
            if (contLogat.imprumuturi.empty()) {
                cout << "  Nu ai nicio carte imprumutata.\n";
                pauseScreen();
                continue;
            }
            cout << "  Cartile tale:\n";
            for (int i = 0; i < (int)contLogat.imprumuturi.size(); i++)
                cout << "  " << i+1 << ". " << contLogat.imprumuturi[i] << "\n";
            cout << "\n";
            string titlu = citesteString("Titlu de returnat: ");
            auto it = find(contLogat.imprumuturi.begin(),
                           contLogat.imprumuturi.end(), titlu);
            if (it != contLogat.imprumuturi.end()) {
                contLogat.imprumuturi.erase(it);
                salveazaConturi(conturi, FISIER_CONTURI);
                cout << "  OK Carte returnata!\n";
            } else {
                cout << "  X Nu ai aceasta carte imprumutata!\n";
            }
            pauseScreen();
        } else if (opt == 6) {
            printHeader("CARTILE MELE - " + contLogat.prenume + " " + contLogat.nume);
            cout << "  Username:       " << contLogat.username << "\n";
            cout << "  Nume complet:   " << contLogat.prenume << " " << contLogat.nume << "\n";
            cout << "  Data nasterii:  " << contLogat.dataNasterii << "\n";
            cout << "  Carti imprumutate: ";
            if (contLogat.imprumuturi.empty()) {
                cout << "Niciuna\n";
            } else {
                cout << "\n";
                for (const auto& t : contLogat.imprumuturi)
                    cout << "    - " << t << "\n";
            }
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

void menuGestionareUseri(Cont& adminLogat) {
    while (true) {
        printHeader("GESTIONARE UTILIZATORI");
        cout << "  1. Lista utilizatori\n";
        cout << "  2. Sterge utilizator\n";
        cout << "  3. Vezi imprumuturile unui utilizator\n";
        cout << "  0. Inapoi\n\n";

        int opt = citesteInt("Optiunea ta: ");

        if (opt == 0) return;

        else if (opt == 1) {
            printHeader("LISTA UTILIZATORI");
            int idx = 1;
            for (const auto& c : conturi) {
                cout << "  " << idx++ << ". "
                     << c.prenume << " " << c.nume
                     << "  (@" << c.username << ")"
                     << "  Nastut: " << c.dataNasterii
                     << (c.esteAdmin ? "  [ADMIN]" : "  [user]") << "\n";
            }
            if (conturi.empty()) cout << "  Nu exista conturi.\n";
            pauseScreen();

        } else if (opt == 2) {
            printHeader("STERGE UTILIZATOR");
            // afiseaza doar userii (nu adminii)
            vector<int> idxUseri;
            for (int i = 0; i < (int)conturi.size(); i++) {
                if (!conturi[i].esteAdmin) idxUseri.push_back(i);
            }
            if (idxUseri.empty()) {
                cout << "  Nu exista utilizatori de sters.\n";
                pauseScreen();
                continue;
            }
            for (int j = 0; j < (int)idxUseri.size(); j++) {
                const auto& c = conturi[idxUseri[j]];
                cout << "  " << j+1 << ". " << c.prenume << " " << c.nume
                     << "  (@" << c.username << ")\n";
            }
            cout << "  0. Anuleaza\n\n";
            int sel = citesteInt("Selectie: ");
            if (sel == 0) { pauseScreen(); continue; }
            if (sel < 1 || sel > (int)idxUseri.size()) {
                cout << "  Selectie invalida!\n";
                pauseScreen();
                continue;
            }
            int idxDeSters = idxUseri[sel - 1];
            string userDeSters = conturi[idxDeSters].username;
            conturi.erase(conturi.begin() + idxDeSters);
            salveazaConturi(conturi, FISIER_CONTURI);
            cout << "\n  OK Utilizatorul @" << userDeSters << " a fost sters.\n";
            pauseScreen();

        } else if (opt == 3) {
            printHeader("IMPRUMUTURI UTILIZATORI");
            bool gasit = false;
            for (const auto& c : conturi) {
                if (c.esteAdmin) continue;
                gasit = true;
                cout << "  Utilizator: " << c.prenume << " " << c.nume
                     << "  (@" << c.username << ")\n";
                if (c.imprumuturi.empty()) {
                    cout << "    (nicio carte imprumutata)\n";
                } else {
                    for (const auto& t : c.imprumuturi)
                        cout << "    - " << t << "\n";
                }
                cout << "\n";
            }
            if (!gasit) cout << "  Nu exista utilizatori.\n";
            pauseScreen();

        } else {
            cout << "  Optiune invalida!\n";
            pauseScreen();
        }
    }
}

void menuAdmin(Biblioteca& bib, Cont& contLogat) {
    while (true) {
        printHeader("ADMIN - " + contLogat.prenume + " " + contLogat.nume);
        cout << "  1. Vezi biblioteca\n";
        cout << "  2. Cauta carte dupa titlu\n";
        cout << "  3. Cauta dupa categorie\n";
        cout << "  4. Adauga carte\n";
        cout << "  5. Sterge carte\n";
        cout << "  6. Gestionare utilizatori\n";
        printSeparator();
        cout << "  0. Logout\n\n";

        int opt = citesteInt("Optiunea ta: ");

        if (opt == 0) {
            cout << "\n  La revedere, Admin " << contLogat.prenume << "!\n";
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
            cautaDupaTip(bib);
        } else if (opt == 4) menuAdauga(bib);
        else if (opt == 5) menuSterge(bib);
        else if (opt == 6) menuGestionareUseri(contLogat);
        else {
            cout << "  Optiune invalida!\n";
            pauseScreen();
        }
    }
}

// ═══════════════════════════════════════════════════════════
//  MAIN
// ═══════════════════════════════════════════════════════════

int main() {
    initializeazaFisiere();
    incarcaConturi(conturi, FISIER_CONTURI);

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
