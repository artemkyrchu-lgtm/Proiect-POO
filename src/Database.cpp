#include "Database.h"
#include "json.hpp"
#include "CarteDigitala.h"
#include "Audiobook.h"
#include "MangaComics.h"
#include "FilmCarte.h"

#include <fstream>
#include <sstream>
#include <iostream>

// ─── helper: citeste tot fisierul intr-un string ──────────────────────────────
static std::string citesteFile(const std::string& cale) {
    std::ifstream f(cale);
    if (!f.is_open()) return "";
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static Json vecToJson(const std::vector<std::string>& v) {
    Json arr = Json::array();
    for (const auto& s : v) arr.push_back(Json(s));
    return arr;
}

static std::vector<std::string> jsonToVec(const Json& arr) {
    std::vector<std::string> v;
    for (const auto& el : arr) v.push_back(el.get_string());
    return v;
}

// ═════════════════════════════════════════════════════════════════════════════
//  SALVARE BIBLIOTECA
// ═════════════════════════════════════════════════════════════════════════════
bool salveazaBiblioteca(const Biblioteca& bib, const std::string& numeFisier) {
    Json root = Json::object();
    Json carti = Json::array();

    for (const auto& c : bib.getCatalog()) {
        Json obj = Json::object();
        obj["tip"]     = Json(c->getTip());
        obj["titlu"]   = Json(c->getTitlu());
        obj["autori"]  = vecToJson(c->getAutori());
        obj["isbn"]    = Json(c->getIsbn());
        obj["volum"]   = Json((int)c->getVolum());
        obj["parte"]   = Json((int)c->getParte());
        obj["editie"]  = Json((int)c->getEditieRevizie());
        obj["detalii"] = Json(c->getDetalii());
        obj["rating"]  = Json(c->getRating());
        obj["serie"]   = Json(c->getSerie());

        const std::string tip = c->getTip();
        if (tip == "CarteDigitala") {
            obj["formatFisier"] = Json(c->getExtra1());
            obj["areDrm"]       = Json(c->getExtraBool());
        } else if (tip == "Audiobook") {
            obj["narator"]      = Json(c->getExtra1());
            obj["durataMinute"] = Json((int)c->getExtraInt());
        } else if (tip == "MangaComics") {
            obj["stilDesen"]    = Json(c->getExtra1());
            obj["esteColor"]    = Json(c->getExtraBool());
        } else if (tip == "FilmCarte") {
            obj["regizor"]      = Json(c->getExtra1());
            obj["anLansare"]    = Json((int)c->getExtraInt());
        }

        carti.push_back(obj);
    }

    root["carti"] = carti;

    std::ofstream f(numeFisier);
    if (!f.is_open()) {
        std::cerr << "  [DB] Nu pot scrie in: " << numeFisier << "\n";
        return false;
    }
    f << root.dump(2);
    f.close();
    return true;
}

// ═════════════════════════════════════════════════════════════════════════════
//  INCARCARE BIBLIOTECA
// ═════════════════════════════════════════════════════════════════════════════
bool incarcaBiblioteca(Biblioteca& bib, const std::string& numeFisier) {
    std::string src = citesteFile(numeFisier);
    if (src.empty()) return false;

    Json root;
    try { root = Json::parse(src); }
    catch (const std::exception& e) {
        std::cerr << "  [DB] Eroare parsare JSON biblioteca: " << e.what() << "\n";
        return false;
    }

    if (!root.contains("carti")) return false;
    const Json& carti = root["carti"];

    for (size_t i = 0; i < carti.size(); i++) {
        const Json& o = carti[i];
        try {
            std::string tip    = o["tip"].get_string();
            std::string titlu  = o["titlu"].get_string();
            auto autori        = jsonToVec(o["autori"]);
            std::string isbn   = o["isbn"].get_string();
            int volum          = (int)o["volum"].get_int();
            int parte          = (int)o["parte"].get_int();
            int editie         = (int)o["editie"].get_int();
            std::string det    = o["detalii"].get_string();
            double rating      = o["rating"].get_double();
            std::string serie  = o["serie"].get_string();

            if (tip == "CarteDigitala") {
                std::string fmt = o["formatFisier"].get_string();
                bool drm        = o["areDrm"].get_bool();
                bib.adaugaCarte(std::make_unique<CarteDigitala>(
                    titlu, autori, isbn, volum, parte, det, rating, editie, serie, fmt, drm));
            } else if (tip == "Audiobook") {
                std::string nar = o["narator"].get_string();
                int durata      = (int)o["durataMinute"].get_int();
                bib.adaugaCarte(std::make_unique<Audiobook>(
                    titlu, autori, isbn, volum, parte, det, rating, editie, serie, nar, durata));
            } else if (tip == "MangaComics") {
                bool color       = o["esteColor"].get_bool();
                std::string stil = o["stilDesen"].get_string();
                bib.adaugaCarte(std::make_unique<MangaComics>(
                    titlu, autori, isbn, volum, parte, det, rating, editie, serie, color, stil));
            } else if (tip == "FilmCarte") {
                std::string reg = o["regizor"].get_string();
                int an          = (int)o["anLansare"].get_int();
                bib.adaugaCarte(std::make_unique<FilmCarte>(
                    titlu, autori, isbn, volum, parte, det, rating, editie, serie, reg, an));
            } else {
                std::cerr << "  [DB] Tip necunoscut: " << tip << "\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "  [DB] Eroare la intrarea " << i << ": " << e.what() << "\n";
        }
    }
    return true;
}

// ═════════════════════════════════════════════════════════════════════════════
//  SALVARE CONTURI
// ═════════════════════════════════════════════════════════════════════════════
bool salveazaConturi(const std::vector<Cont>& conturi, const std::string& numeFisier) {
    Json root = Json::object();
    Json arr  = Json::array();

    for (const auto& c : conturi) {
        Json obj = Json::object();
        obj["username"]      = Json(c.username);
        obj["parola"]        = Json(c.parola);
        obj["esteAdmin"]     = Json(c.esteAdmin);
        obj["nume"]          = Json(c.nume);
        obj["prenume"]       = Json(c.prenume);
        obj["dataNasterii"]  = Json(c.dataNasterii);
        // salveaza imprumuturi
        Json imp = Json::array();
        for (const auto& t : c.imprumuturi) imp.push_back(Json(t));
        obj["imprumuturi"]   = imp;
        arr.push_back(obj);
    }
    root["conturi"] = arr;

    std::ofstream f(numeFisier);
    if (!f.is_open()) {
        std::cerr << "  [DB] Nu pot scrie in: " << numeFisier << "\n";
        return false;
    }
    f << root.dump(2);
    f.close();
    return true;
}

// ═════════════════════════════════════════════════════════════════════════════
//  INCARCARE CONTURI
// ═════════════════════════════════════════════════════════════════════════════
bool incarcaConturi(std::vector<Cont>& conturi, const std::string& numeFisier) {
    std::string src = citesteFile(numeFisier);
    if (src.empty()) return false;

    Json root;
    try { root = Json::parse(src); }
    catch (const std::exception& e) {
        std::cerr << "  [DB] Eroare parsare JSON conturi: " << e.what() << "\n";
        return false;
    }

    if (!root.contains("conturi")) return false;
    const Json& arr = root["conturi"];

    for (size_t i = 0; i < arr.size(); i++) {
        const Json& o = arr[i];
        try {
            Cont c;
            c.username     = o["username"].get_string();
            c.parola       = o["parola"].get_string();
            c.esteAdmin    = o["esteAdmin"].get_bool();
            // campuri noi - compatibil cu fisiere vechi (optional)
            if (o.contains("nume"))         c.nume         = o["nume"].get_string();
            if (o.contains("prenume"))      c.prenume      = o["prenume"].get_string();
            if (o.contains("dataNasterii")) c.dataNasterii = o["dataNasterii"].get_string();
            if (o.contains("imprumuturi")) {
                const Json& imp = o["imprumuturi"];
                for (size_t j = 0; j < imp.size(); j++)
                    c.imprumuturi.push_back(imp[j].get_string());
            }
            conturi.push_back(c);
        } catch (const std::exception& e) {
            std::cerr << "  [DB] Eroare cont " << i << ": " << e.what() << "\n";
        }
    }
    return true;
}

// ═════════════════════════════════════════════════════════════════════════════
//  INITIALIZARE fisiere default
// ═════════════════════════════════════════════════════════════════════════════
void initializeazaFisiere() {
    {
        std::ifstream test(FISIER_CONTURI);
        if (!test.is_open()) {
            std::vector<Cont> defaults = {
                {"admin", "1234", true,  "Ion",    "Popescu", "01.01.1980", {}},
                {"user",  "1234", false, "Maria",  "Ionescu", "15.06.1995", {}}
            };
            if (salveazaConturi(defaults, FISIER_CONTURI))
                std::cout << "  [DB] conturi.json creat cu conturi default.\n";
        }
    }
    {
        std::ifstream test(FISIER_BIBLIOTECA);
        if (!test.is_open()) {
            std::ofstream f(FISIER_BIBLIOTECA);
            if (f.is_open()) {
                f << "{\n  \"carti\": []\n}\n";
                std::cout << "  [DB] biblioteca.json creat (gol).\n";
            }
        }
    }
}
