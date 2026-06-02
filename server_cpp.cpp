#ifdef HAVE_SSL
#  define CPPHTTPLIB_OPENSSL_SUPPORT
#endif
#include "httplib.h"
#include "nlohmann_json.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <mutex>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <iostream>

using json = nlohmann::json;
using namespace httplib;

// ─── Config ───────────────────────────────────────────────────────────────────
const std::string BIBLIOTECA_PATH = "biblioteca.json";
const std::string CONTURI_PATH    = "conturi.json";
const std::string STATIC_DIR      = "web/public";
const std::string ADMIN_CODE      = "BIBLIO_ADMIN_2024";
std::string       OMDB_KEY        = "8461fe15";
const int         PORT            = 3000;

// ─── Session store ─────────────────────────────────────────────────────────────
std::map<std::string, std::string> sessions; // session_id -> username
std::mutex sessions_mtx;

// ─── File mutex ────────────────────────────────────────────────────────────────
std::mutex file_mtx;

// ─── Date helpers ──────────────────────────────────────────────────────────────
std::string dateStr(int offsetDays = 0) {
    auto now = std::chrono::system_clock::now();
    now += std::chrono::hours(offsetDays * 24);
    auto t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::gmtime(&t);
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d");
    return ss.str();
}

// ─── URL decode ────────────────────────────────────────────────────────────────
std::string urlDecode(const std::string& s) {
    std::string out;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '%' && i + 2 < s.size()) {
            int c = std::stoi(s.substr(i + 1, 2), nullptr, 16);
            out += (char)c;
            i += 2;
        } else if (s[i] == '+') {
            out += ' ';
        } else {
            out += s[i];
        }
    }
    return out;
}

std::string urlEncode(const std::string& s) {
    std::string out;
    for (unsigned char c : s) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            out += c;
        } else {
            char buf[4];
            snprintf(buf, sizeof(buf), "%%%02X", c);
            out += buf;
        }
    }
    return out;
}

// ─── File helpers ──────────────────────────────────────────────────────────────
json normImp(const json& imp) {
    if (imp.is_string())
        return {{"titlu", imp.get<std::string>()}, {"dataImprumut", dateStr()}, {"dataReturnare", dateStr(14)}};
    return imp;
}

json readBiblioteca() {
    std::lock_guard<std::mutex> lock(file_mtx);
    std::ifstream f(BIBLIOTECA_PATH);
    json j; f >> j;
    return j["carti"];
}

void writeBiblioteca(const json& carti) {
    std::lock_guard<std::mutex> lock(file_mtx);
    std::ofstream f(BIBLIOTECA_PATH);
    f << json{{"carti", carti}}.dump(2);
}

json readConturi() {
    std::lock_guard<std::mutex> lock(file_mtx);
    std::ifstream f(CONTURI_PATH);
    json j; f >> j;
    auto conturi = j["conturi"];
    for (auto& c : conturi) {
        if (!c.contains("imprumuturi")) c["imprumuturi"] = json::array();
        json norm = json::array();
        for (auto& i : c["imprumuturi"]) norm.push_back(normImp(i));
        c["imprumuturi"] = norm;
    }
    return conturi;
}

void writeConturi(const json& conturi) {
    std::lock_guard<std::mutex> lock(file_mtx);
    std::ofstream f(CONTURI_PATH);
    f << json{{"conturi", conturi}}.dump(2);
}

// ─── Session helpers ───────────────────────────────────────────────────────────
std::string generateSid() {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<int> dist(0, 15);
    static const char* hex = "0123456789abcdef";
    std::string id(32, '0');
    for (auto& c : id) c = hex[dist(rng)];
    return id;
}

std::string getCookie(const Request& req, const std::string& name) {
    auto it = req.headers.find("Cookie");
    if (it == req.headers.end()) return "";
    std::string search = name + "=";
    auto pos = it->second.find(search);
    if (pos == std::string::npos) return "";
    pos += search.size();
    auto end = it->second.find(';', pos);
    return it->second.substr(pos, end == std::string::npos ? std::string::npos : end - pos);
}

std::string getSessionUser(const Request& req) {
    // Try X-Session-Token header first (used by frontend)
    std::string sid;
    auto hit = req.headers.find("X-Session-Token");
    if (hit != req.headers.end() && !hit->second.empty())
        sid = hit->second;
    else
        sid = getCookie(req, "sid");
    if (sid.empty()) return "";
    std::lock_guard<std::mutex> lock(sessions_mtx);
    auto it = sessions.find(sid);
    return it != sessions.end() ? it->second : "";
}

std::string setSession(Response& res, const std::string& username) {
    std::string sid = generateSid();
    {
        std::lock_guard<std::mutex> lock(sessions_mtx);
        sessions[sid] = username;
    }
    res.set_header("Set-Cookie", "sid=" + sid + "; Path=/; HttpOnly");
    return sid;
}

void clearSession(const Request& req, Response& res) {
    std::string sid = getCookie(req, "sid");
    if (!sid.empty()) {
        std::lock_guard<std::mutex> lock(sessions_mtx);
        sessions.erase(sid);
    }
    res.set_header("Set-Cookie", "sid=; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT");
}

// ─── Auth helpers ──────────────────────────────────────────────────────────────
bool isAuth(const Request& req)  { return !getSessionUser(req).empty(); }

json getContByUsername(const std::string& username) {
    for (auto& c : readConturi())
        if (c["username"] == username) return c;
    return nullptr;
}

bool isAdmin(const Request& req) {
    auto c = getContByUsername(getSessionUser(req));
    return !c.is_null() && c.value("esteAdmin", false);
}

bool isDirector(const Request& req) {
    auto c = getContByUsername(getSessionUser(req));
    return !c.is_null() && c.value("esteAdmin", false) && c.value("functie", "") == "Director";
}

// ─── safeUser ──────────────────────────────────────────────────────────────────
json safeUser(const json& c) {
    json u;
    u["username"]     = c.value("username", "");
    u["nume"]         = c.value("nume", "");
    u["prenume"]      = c.value("prenume", "");
    u["dataNasterii"] = c.value("dataNasterii", "");
    u["esteAdmin"]    = c.value("esteAdmin", false);
    u["email"]        = c.value("email", "");
    u["telefon"]      = c.value("telefon", "");
    u["functie"]      = c.value("functie", "");
    u["imprumuturi"]  = c.value("imprumuturi", json::array());
    u["favorite"]     = c.value("favorite", json::array());
    u["salariu"]      = c.contains("salariu") ? c["salariu"] : json(nullptr);
    return u;
}

// ─── JSON response helper ──────────────────────────────────────────────────────
void jr(Response& res, const json& j, int status = 200) {
    res.status = status;
    res.set_header("Content-Type", "application/json");
    res.body = j.dump();
}

// ─── main ──────────────────────────────────────────────────────────────────────
int main() {
    Server svr;

    if (!svr.set_mount_point("/", STATIC_DIR.c_str())) {
        std::cerr << "  Eroare: nu am gasit directorul " << STATIC_DIR << "\n";
        return 1;
    }

    // ── POST /api/login ────────────────────────────────────────────────────────
    svr.Post("/api/login", [](const Request& req, Response& res) {
        auto body = json::parse(req.body, nullptr, false);
        if (body.is_discarded()) return jr(res, {{"error", "Bad request"}}, 400);
        auto conturi = readConturi();
        for (auto& c : conturi) {
            if (c["username"] == body.value("username", "") && c["parola"] == body.value("parola", "")) {
                auto sid = setSession(res, c["username"].get<std::string>());
                auto u = safeUser(c); u["sid"] = sid;
                return jr(res, u);
            }
        }
        jr(res, {{"error", "Username sau parola incorecta"}}, 401);
    });

    // ── POST /api/logout ───────────────────────────────────────────────────────
    svr.Post("/api/logout", [](const Request& req, Response& res) {
        clearSession(req, res);
        jr(res, {{"ok", true}});
    });

    // ── POST /api/register ─────────────────────────────────────────────────────
    svr.Post("/api/register", [](const Request& req, Response& res) {
        auto body = json::parse(req.body, nullptr, false);
        if (body.is_discarded()) return jr(res, {{"error", "Bad request"}}, 400);
        std::string username = body.value("username", ""), parola = body.value("parola", "");
        std::string nume = body.value("nume", ""), prenume = body.value("prenume", "");
        if (username.empty() || parola.empty() || nume.empty() || prenume.empty())
            return jr(res, {{"error", "Toate campurile sunt obligatorii"}}, 400);
        auto conturi = readConturi();
        for (auto& c : conturi)
            if (c["username"] == username)
                return jr(res, {{"error", "Username deja folosit"}}, 400);
        json cont = {{"username", username}, {"parola", parola}, {"esteAdmin", false},
                     {"nume", nume}, {"prenume", prenume}, {"dataNasterii", body.value("dataNasterii", "")},
                     {"imprumuturi", json::array()}, {"favorite", json::array()}};
        conturi.push_back(cont);
        writeConturi(conturi);
        { auto sid = setSession(res, username); auto u = safeUser(cont); u["sid"] = sid; jr(res, u); }
    });

    // ── POST /api/register-admin ───────────────────────────────────────────────
    svr.Post("/api/register-admin", [](const Request& req, Response& res) {
        auto body = json::parse(req.body, nullptr, false);
        if (body.is_discarded()) return jr(res, {{"error", "Bad request"}}, 400);
        std::string username = body.value("username", ""), parola = body.value("parola", "");
        std::string nume = body.value("nume", ""), prenume = body.value("prenume", "");
        if (username.empty() || parola.empty() || nume.empty() || prenume.empty())
            return jr(res, {{"error", "Prenume, nume, username si parola sunt obligatorii"}}, 400);
        if (body.value("cod", "") != ADMIN_CODE)
            return jr(res, {{"error", "Cod de acces incorect"}}, 403);
        auto conturi = readConturi();
        for (auto& c : conturi)
            if (c["username"] == username)
                return jr(res, {{"error", "Username deja folosit"}}, 400);
        json cont = {{"username", username}, {"parola", parola}, {"esteAdmin", true},
                     {"nume", nume}, {"prenume", prenume}, {"dataNasterii", body.value("dataNasterii", "")},
                     {"email", body.value("email", "")}, {"telefon", body.value("telefon", "")},
                     {"functie", body.value("functie", "Bibliotecar")},
                     {"imprumuturi", json::array()}, {"favorite", json::array()}};
        conturi.push_back(cont);
        writeConturi(conturi);
        { auto sid = setSession(res, username); auto u = safeUser(cont); u["sid"] = sid; jr(res, u); }
    });

    // ── GET /api/me ────────────────────────────────────────────────────────────
    svr.Get("/api/me", [](const Request& req, Response& res) {
        if (!isAuth(req)) return jr(res, {{"error", "Neautentificat"}}, 401);
        auto cont = getContByUsername(getSessionUser(req));
        if (cont.is_null()) return jr(res, {{"error", "Cont negasit"}}, 404);
        jr(res, safeUser(cont));
    });

    // ── GET /api/carti ─────────────────────────────────────────────────────────
    svr.Get("/api/carti", [](const Request& req, Response& res) {
        auto carti = readBiblioteca();

        std::string tip = req.has_param("tip") ? req.get_param_value("tip") : "";
        if (!tip.empty() && tip != "all") {
            json f = json::array();
            for (auto& c : carti) if (c.value("tip", "") == tip) f.push_back(c);
            carti = f;
        }

        std::string q = req.has_param("q") ? req.get_param_value("q") : "";
        if (!q.empty()) {
            std::string ql = q;
            std::transform(ql.begin(), ql.end(), ql.begin(), ::tolower);
            json f = json::array();
            for (auto& c : carti) {
                auto lc = [](std::string s) { std::transform(s.begin(), s.end(), s.begin(), ::tolower); return s; };
                bool m = lc(c.value("titlu", "")).find(ql) != std::string::npos ||
                         lc(c.value("serie", "")).find(ql) != std::string::npos;
                if (!m) for (auto& a : c.value("autori", json::array()))
                    if (lc(a.get<std::string>()).find(ql) != std::string::npos) { m = true; break; }
                if (m) f.push_back(c);
            }
            carti = f;
        }

        auto conturi = readConturi();
        std::map<std::string, int> bmap;
        for (auto& c : conturi)
            for (auto& i : c.value("imprumuturi", json::array()))
                bmap[i.value("titlu", "")]++;

        for (auto& c : carti)
            c["copiiDisponibile"] = std::max(0, c.value("copii", 1) - (bmap.count(c.value("titlu","")) ? bmap[c.value("titlu","")] : 0));

        jr(res, {{"carti", carti}});
    });

    // ── POST /api/carti ────────────────────────────────────────────────────────
    svr.Post("/api/carti", [](const Request& req, Response& res) {
        if (!isAdmin(req)) return jr(res, {{"error", "Acces interzis"}}, 403);
        auto carte = json::parse(req.body, nullptr, false);
        if (carte.is_discarded() || !carte.contains("titlu") || !carte.contains("tip"))
            return jr(res, {{"error", "Titlu si tip sunt obligatorii"}}, 400);
        auto carti = readBiblioteca();
        for (auto& c : carti) if (c["titlu"] == carte["titlu"]) return jr(res, {{"error", "Exista deja o carte cu acest titlu"}}, 400);
        carti.push_back(carte);
        writeBiblioteca(carti);
        jr(res, {{"ok", true}});
    });

    // ── PUT /api/carti/:titlu ──────────────────────────────────────────────────
    svr.Put(R"(/api/carti/(.+))", [](const Request& req, Response& res) {
        if (!isAdmin(req)) return jr(res, {{"error", "Acces interzis"}}, 403);
        std::string titluVechi = urlDecode(req.matches[1]);
        auto body = json::parse(req.body, nullptr, false);
        if (body.is_discarded()) return jr(res, {{"error", "Bad request"}}, 400);

        auto carti = readBiblioteca();
        int idx = -1;
        for (int i = 0; i < (int)carti.size(); i++) if (carti[i]["titlu"] == titluVechi) { idx = i; break; }
        if (idx == -1) return jr(res, {{"error", "Cartea nu a fost gasita"}}, 404);

        std::string titluNou = body.value("titlu", titluVechi);
        if (titluNou != titluVechi) {
            for (auto& c : carti) if (c["titlu"] == titluNou) return jr(res, {{"error", "Exista deja o carte cu titlul nou"}}, 400);
            auto conturi = readConturi();
            bool changed = false;
            for (auto& c : conturi) {
                for (auto& i : c["imprumuturi"]) if (i.value("titlu","") == titluVechi) { i["titlu"] = titluNou; changed = true; }
                if (c.contains("favorite")) for (auto& f : c["favorite"]) if (f == titluVechi) { f = titluNou; changed = true; }
            }
            if (changed) writeConturi(conturi);
        }

        json prevRec = carti[idx].value("recenzii", json::array());
        carti[idx] = body;
        if (!prevRec.empty()) carti[idx]["recenzii"] = prevRec;
        writeBiblioteca(carti);
        jr(res, {{"ok", true}});
    });

    // ── DELETE /api/carti/:titlu ───────────────────────────────────────────────
    svr.Delete(R"(/api/carti/(.+))", [](const Request& req, Response& res) {
        if (!isAdmin(req)) return jr(res, {{"error", "Acces interzis"}}, 403);
        std::string titlu = urlDecode(req.matches[1]);
        auto carti = readBiblioteca();
        int before = carti.size();
        json f = json::array();
        for (auto& c : carti) if (c["titlu"] != titlu) f.push_back(c);
        if ((int)f.size() == before) return jr(res, {{"error", "Cartea nu a fost gasita"}}, 404);
        writeBiblioteca(f);

        auto conturi = readConturi();
        bool changed = false;
        for (auto& c : conturi) {
            if (c.contains("favorite")) {
                int prev = c["favorite"].size();
                json ff = json::array();
                for (auto& x : c["favorite"]) if (x != titlu) ff.push_back(x);
                if ((int)ff.size() != prev) { c["favorite"] = ff; changed = true; }
            }
        }
        if (changed) writeConturi(conturi);
        jr(res, {{"ok", true}});
    });

    // ── POST /api/imprumut ─────────────────────────────────────────────────────
    svr.Post("/api/imprumut", [](const Request& req, Response& res) {
        if (!isAuth(req)) return jr(res, {{"error", "Neautentificat"}}, 401);
        auto body = json::parse(req.body, nullptr, false);
        std::string titlu = body.value("titlu", "");

        auto carti = readBiblioteca();
        auto conturi = readConturi();

        for (auto& c : carti) {
            if (c["titlu"] == titlu && c.contains("copii")) {
                int copii = c["copii"], borrowed = 0;
                for (auto& u : conturi)
                    for (auto& i : u["imprumuturi"])
                        if (i.value("titlu","") == titlu) borrowed++;
                if (borrowed >= copii) return jr(res, {{"error", "Nu mai sunt copii disponibile"}}, 400);
            }
        }

        std::string username = getSessionUser(req);
        int idx = -1;
        for (int i = 0; i < (int)conturi.size(); i++) if (conturi[i]["username"] == username) { idx = i; break; }
        if (idx == -1) return jr(res, {{"error", "Cont negasit"}}, 404);
        for (auto& i : conturi[idx]["imprumuturi"]) if (i.value("titlu","") == titlu) return jr(res, {{"error", "Ai deja aceasta carte imprumutata"}}, 400);

        conturi[idx]["imprumuturi"].push_back({{"titlu", titlu}, {"dataImprumut", dateStr()}, {"dataReturnare", dateStr(14)}});
        writeConturi(conturi);
        jr(res, {{"ok", true}, {"imprumuturi", conturi[idx]["imprumuturi"]}});
    });

    // ── DELETE /api/imprumut/:titlu ────────────────────────────────────────────
    svr.Delete(R"(/api/imprumut/(.+))", [](const Request& req, Response& res) {
        if (!isAuth(req)) return jr(res, {{"error", "Neautentificat"}}, 401);
        std::string titlu = urlDecode(req.matches[1]);
        std::string username = getSessionUser(req);
        auto conturi = readConturi();
        int idx = -1;
        for (int i = 0; i < (int)conturi.size(); i++) if (conturi[i]["username"] == username) { idx = i; break; }
        if (idx == -1) return jr(res, {{"error", "Cont negasit"}}, 404);
        auto& impr = conturi[idx]["imprumuturi"];
        int ii = -1;
        for (int i = 0; i < (int)impr.size(); i++) if (impr[i].value("titlu","") == titlu) { ii = i; break; }
        if (ii == -1) return jr(res, {{"error", "Nu ai aceasta carte imprumutata"}}, 400);
        impr.erase(ii);
        writeConturi(conturi);
        jr(res, {{"ok", true}, {"imprumuturi", impr}});
    });

    // ── GET /api/users ─────────────────────────────────────────────────────────
    svr.Get("/api/users", [](const Request& req, Response& res) {
        if (!isAdmin(req)) return jr(res, {{"error", "Acces interzis"}}, 403);
        json result = json::array();
        for (auto& c : readConturi()) result.push_back(safeUser(c));
        jr(res, {{"conturi", result}});
    });

    // ── PUT /api/users/:username ───────────────────────────────────────────────
    svr.Put(R"(/api/users/(.+))", [](const Request& req, Response& res) {
        if (!isDirector(req)) return jr(res, {{"error", "Acces interzis — necesita rol Director"}}, 403);
        std::string username = req.matches[1];
        auto body = json::parse(req.body, nullptr, false);
        auto conturi = readConturi();
        int idx = -1;
        for (int i = 0; i < (int)conturi.size(); i++) if (conturi[i]["username"] == username) { idx = i; break; }
        if (idx == -1) return jr(res, {{"error", "Utilizatorul nu a fost gasit"}}, 404);
        auto& c = conturi[idx];
        if (body.contains("prenume") && !body["prenume"].is_null()) c["prenume"] = body["prenume"];
        if (body.contains("nume") && !body["nume"].is_null()) c["nume"] = body["nume"];
        if (body.contains("dataNasterii")) c["dataNasterii"] = body["dataNasterii"];
        if (body.contains("parola") && !body["parola"].get<std::string>().empty()) c["parola"] = body["parola"];
        if (body.contains("email")) c["email"] = body["email"];
        if (body.contains("telefon")) c["telefon"] = body["telefon"];
        if (body.contains("functie") && !body["functie"].is_null()) c["functie"] = body["functie"];
        if (body.contains("salariu")) c["salariu"] = body["salariu"];
        writeConturi(conturi);
        jr(res, {{"ok", true}, {"user", safeUser(c)}});
    });

    // ── DELETE /api/users/:username ────────────────────────────────────────────
    svr.Delete(R"(/api/users/(.+))", [](const Request& req, Response& res) {
        if (!isDirector(req)) return jr(res, {{"error", "Acces interzis — necesita rol Director"}}, 403);
        std::string username = req.matches[1];
        if (username == getSessionUser(req)) return jr(res, {{"error", "Nu te poti sterge pe tine insuti"}}, 400);
        auto conturi = readConturi();
        int before = conturi.size();
        json f = json::array();
        for (auto& c : conturi) if (c["username"] != username) f.push_back(c);
        if ((int)f.size() == before) return jr(res, {{"error", "Utilizatorul nu a fost gasit"}}, 404);
        writeConturi(f);
        jr(res, {{"ok", true}});
    });

    // ── POST /api/amenzi/stinge ────────────────────────────────────────────────
    svr.Post("/api/amenzi/stinge", [](const Request& req, Response& res) {
        if (!isAdmin(req)) return jr(res, {{"error", "Acces interzis"}}, 403);
        auto body = json::parse(req.body, nullptr, false);
        std::string username = body.value("username",""), titlu = body.value("titlu","");
        auto conturi = readConturi();
        int idx = -1;
        for (int i = 0; i < (int)conturi.size(); i++) if (conturi[i]["username"] == username) { idx = i; break; }
        if (idx == -1) return jr(res, {{"error", "Utilizatorul nu a fost gasit"}}, 404);
        bool found = false;
        for (auto& i : conturi[idx]["imprumuturi"]) if (i.value("titlu","") == titlu) { i["amendaStinsa"] = true; found = true; break; }
        if (!found) return jr(res, {{"error", "Imprumut negasit"}}, 404);
        writeConturi(conturi);
        jr(res, {{"ok", true}});
    });

    // ── POST /api/recenzii/:titlu ──────────────────────────────────────────────
    svr.Post(R"(/api/recenzii/(.+))", [](const Request& req, Response& res) {
        if (!isAuth(req)) return jr(res, {{"error", "Neautentificat"}}, 401);
        std::string titlu = urlDecode(req.matches[1]);
        auto body = json::parse(req.body, nullptr, false);
        int rating = body.value("rating", 0);
        if (rating < 1 || rating > 5) return jr(res, {{"error", "Rating invalid (1-5)"}}, 400);
        auto carti = readBiblioteca();
        int idx = -1;
        for (int i = 0; i < (int)carti.size(); i++) if (carti[i]["titlu"] == titlu) { idx = i; break; }
        if (idx == -1) return jr(res, {{"error", "Cartea nu a fost gasita"}}, 404);
        if (!carti[idx].contains("recenzii")) carti[idx]["recenzii"] = json::array();
        std::string comment = body.value("comment","");
        if (comment.size() > 500) comment = comment.substr(0, 500);
        json rec = {{"username", getSessionUser(req)}, {"rating", rating}, {"comment", comment}, {"data", dateStr()}};
        bool replaced = false;
        for (auto& r : carti[idx]["recenzii"]) if (r["username"] == rec["username"]) { r = rec; replaced = true; break; }
        if (!replaced) carti[idx]["recenzii"].push_back(rec);
        writeBiblioteca(carti);
        jr(res, {{"ok", true}, {"recenzii", carti[idx]["recenzii"]}});
    });

    // ── DELETE /api/recenzii/:titlu ────────────────────────────────────────────
    svr.Delete(R"(/api/recenzii/(.+))", [](const Request& req, Response& res) {
        if (!isAuth(req)) return jr(res, {{"error", "Neautentificat"}}, 401);
        std::string titlu = urlDecode(req.matches[1]);
        std::string username = getSessionUser(req);
        auto carti = readBiblioteca();
        int idx = -1;
        for (int i = 0; i < (int)carti.size(); i++) if (carti[i]["titlu"] == titlu) { idx = i; break; }
        if (idx == -1) return jr(res, {{"error", "Cartea nu a fost gasita"}}, 404);
        json f = json::array();
        for (auto& r : carti[idx].value("recenzii", json::array())) if (r["username"] != username) f.push_back(r);
        carti[idx]["recenzii"] = f;
        writeBiblioteca(carti);
        jr(res, {{"ok", true}, {"recenzii", f}});
    });

    // ── POST /api/favorite ─────────────────────────────────────────────────────
    svr.Post("/api/favorite", [](const Request& req, Response& res) {
        if (!isAuth(req)) return jr(res, {{"error", "Neautentificat"}}, 401);
        auto body = json::parse(req.body, nullptr, false);
        std::string titlu = body.value("titlu","");
        if (titlu.empty()) return jr(res, {{"error", "Titlu lipsa"}}, 400);
        std::string username = getSessionUser(req);
        auto conturi = readConturi();
        int idx = -1;
        for (int i = 0; i < (int)conturi.size(); i++) if (conturi[i]["username"] == username) { idx = i; break; }
        if (idx == -1) return jr(res, {{"error", "Cont negasit"}}, 404);
        if (!conturi[idx].contains("favorite")) conturi[idx]["favorite"] = json::array();
        bool exists = false;
        for (auto& f : conturi[idx]["favorite"]) if (f == titlu) { exists = true; break; }
        if (!exists) conturi[idx]["favorite"].push_back(titlu);
        writeConturi(conturi);
        jr(res, {{"ok", true}, {"favorite", conturi[idx]["favorite"]}});
    });

    // ── DELETE /api/favorite/:titlu ────────────────────────────────────────────
    svr.Delete(R"(/api/favorite/(.+))", [](const Request& req, Response& res) {
        if (!isAuth(req)) return jr(res, {{"error", "Neautentificat"}}, 401);
        std::string titlu = urlDecode(req.matches[1]);
        std::string username = getSessionUser(req);
        auto conturi = readConturi();
        int idx = -1;
        for (int i = 0; i < (int)conturi.size(); i++) if (conturi[i]["username"] == username) { idx = i; break; }
        if (idx == -1) return jr(res, {{"error", "Cont negasit"}}, 404);
        json f = json::array();
        for (auto& x : conturi[idx].value("favorite", json::array())) if (x != titlu) f.push_back(x);
        conturi[idx]["favorite"] = f;
        writeConturi(conturi);
        jr(res, {{"ok", true}, {"favorite", f}});
    });

    // ── GET /api/search/book (Open Library) ───────────────────────────────────
    svr.Get("/api/search/book", [](const Request& req, Response& res) {
        std::string q = req.has_param("q") ? req.get_param_value("q") : "";
        if (q.empty()) { jr(res, {{"results", json::array()}}); return; }
#ifdef HAVE_SSL
        try {
            SSLClient cli("openlibrary.org");
            cli.set_connection_timeout(5); cli.set_read_timeout(8);
            auto r = cli.Get(("/search.json?q=" + urlEncode(q) + "&fields=title,author_name,isbn,cover_i,first_publish_year,series,edition_count&limit=8").c_str());
            if (!r || r->status != 200) { jr(res, {{"results", json::array()}}); return; }
            auto data = json::parse(r->body, nullptr, false);
            if (data.is_discarded()) { jr(res, {{"results", json::array()}}); return; }
            json results = json::array();
            for (auto& doc : data.value("docs", json::array())) {
                std::string imagine;
                if (doc.contains("cover_i") && !doc["cover_i"].is_null())
                    imagine = "https://covers.openlibrary.org/b/id/" + std::to_string(doc["cover_i"].get<int>()) + "-M.jpg";
                std::string isbn = (doc.contains("isbn") && !doc["isbn"].empty()) ? doc["isbn"][0].get<std::string>() : "";
                std::string serie = (doc.contains("series") && !doc["series"].empty()) ? doc["series"][0].get<std::string>() : "";
                json item; item["titlu"] = doc.value("title",""); item["autori"] = doc.value("author_name", json::array());
                item["isbn"] = isbn; item["serie"] = serie; item["imagine"] = imagine;
                item["an"] = doc.value("first_publish_year", 0); item["editie"] = doc.value("edition_count", 1); item["detalii"] = "";
                results.push_back(item);
            }
            jr(res, {{"results", results}});
        } catch (...) {
            jr(res, {{"error", "Eroare la cautarea cartilor (Open Library)"}}, 500);
        }
#else
        jr(res, {{"error", "SSL_NOT_AVAILABLE"}}, 503);
#endif
    });

    // ── GET /api/search/film (OMDB) ────────────────────────────────────────────
    svr.Get("/api/search/film", [](const Request& req, Response& res) {
        std::string q = req.has_param("q") ? req.get_param_value("q") : "";
        if (q.empty()) { jr(res, {{"results", json::array()}}); return; }
#ifdef HAVE_SSL
        if (OMDB_KEY == "YOUR_OMDB_KEY") { jr(res, {{"error", "OMDB_KEY_MISSING"}}, 503); return; }
        try {
            SSLClient cli("www.omdbapi.com");
            cli.set_connection_timeout(5); cli.set_read_timeout(8);
            auto r = cli.Get(("/?s=" + urlEncode(q) + "&type=movie&apikey=" + OMDB_KEY).c_str());
            if (!r || r->status != 200) { jr(res, {{"results", json::array()}}); return; }
            auto searchData = json::parse(r->body, nullptr, false);
            if (searchData.is_discarded() || searchData.value("Response","") == "False") { jr(res, {{"results", json::array()}}); return; }
            json results = json::array();
            auto top = searchData.value("Search", json::array());
            int limit = std::min((int)top.size(), 6);
            for (int i = 0; i < limit; i++) {
                auto& it = top[i];
                std::string poster = it.value("Poster",""); if (poster == "N/A") poster = "";
                int an = 0; try { an = std::stoi(it.value("Year","0")); } catch (...) {}
                json entry; entry["titlu"] = it.value("Title",""); entry["autori"] = json::array();
                entry["regizor"] = ""; entry["anLansare"] = an; entry["imagine"] = poster;
                entry["rating"] = 0; entry["detalii"] = ""; entry["isbn"] = "";
                auto dr = cli.Get(("/?i=" + it.value("imdbID","") + "&apikey=" + OMDB_KEY).c_str());
                if (dr && dr->status == 200) {
                    auto d = json::parse(dr->body, nullptr, false);
                    if (!d.is_discarded()) {
                        std::string p = d.value("Poster",""); if (p=="N/A") p="";
                        std::string reg = d.value("Director",""); if (reg=="N/A") reg="";
                        std::string plot = d.value("Plot",""); if (plot=="N/A") plot="";
                        double rat = 0; try { rat = std::min(10.0, std::stod(d.value("imdbRating","0"))); } catch(...) {}
                        int year = 0; try { year = std::stoi(d.value("Year","0")); } catch(...) {}
                        json autori = json::array();
                        std::string writer = d.value("Writer","");
                        if (writer != "N/A" && !writer.empty()) {
                            std::istringstream ss(writer); std::string tok;
                            while (std::getline(ss, tok, ',')) {
                                tok.erase(0, tok.find_first_not_of(" "));
                                tok.erase(tok.find_last_not_of(" ")+1);
                                autori.push_back(tok);
                            }
                        }
                        entry["titlu"] = d.value("Title", it.value("Title",""));
                        entry["autori"] = autori; entry["regizor"] = reg; entry["anLansare"] = year;
                        entry["imagine"] = p; entry["rating"] = rat; entry["detalii"] = plot;
                    }
                }
                results.push_back(entry);
            }
            jr(res, {{"results", results}});
        } catch (...) {
            jr(res, {{"error", "Eroare la cautarea filmelor (OMDB)"}}, 500);
        }
#else
        jr(res, {{"error", "OMDB_KEY_MISSING"}}, 503);
#endif
    });

    // ── POST /api/admin/fetch-covers ───────────────────────────────────────────
    svr.Post("/api/admin/fetch-covers", [](const Request& req, Response& res) {
        if (!isAdmin(req)) { jr(res, {{"error", "Acces interzis"}}, 403); return; }
#ifdef HAVE_SSL
        auto carti = readBiblioteca();
        int found = 0, skipped = 0;
        SSLClient cli("openlibrary.org");
        cli.set_connection_timeout(5); cli.set_read_timeout(8);
        for (auto& c : carti) {
            if (!c.value("imagine","").empty()) { skipped++; continue; }
            try {
                auto r = cli.Get(("/search.json?q=" + urlEncode(c.value("titlu","")) + "&fields=cover_i&limit=1").c_str());
                if (r && r->status == 200) {
                    auto data = json::parse(r->body, nullptr, false);
                    if (!data.is_discarded()) {
                        auto docs = data.value("docs", json::array());
                        if (!docs.empty() && docs[0].contains("cover_i") && !docs[0]["cover_i"].is_null()) {
                            c["imagine"] = "https://covers.openlibrary.org/b/id/" + std::to_string(docs[0]["cover_i"].get<int>()) + "-M.jpg";
                            found++;
                        }
                    }
                }
            } catch (...) {}
        }
        writeBiblioteca(carti);
        jr(res, {{"ok", true}, {"found", found}, {"skipped", skipped}, {"total", (int)carti.size()}});
#else
        jr(res, {{"ok", true}, {"found", 0}, {"skipped", 0}, {"total", 0}});
#endif
    });

    std::cout << "\n  Biblioteca C++ ruleaza la http://localhost:" << PORT << "\n\n";
    svr.listen("0.0.0.0", PORT);
    return 0;
}
