#pragma once
// ============================================================
//  json.hpp  –  mini JSON parser/writer (no external deps)
//  Suporta: null, bool, int, double, string, array, object
// ============================================================

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <cctype>

class Json {
public:
    enum Type { Null, Bool, Int, Double, String, Array, Object };

    Json()                       : type_(Null)   {}
    Json(bool v)                 : type_(Bool),   bval_(v) {}
    Json(int v)                  : type_(Int),    ival_(v) {}
    Json(long long v)            : type_(Int),    ival_(v) {}
    Json(double v)               : type_(Double), dval_(v) {}
    Json(const char* v)          : type_(String), sval_(v) {}
    Json(const std::string& v)   : type_(String), sval_(v) {}

    static Json array()  { Json j; j.type_ = Array;  return j; }
    static Json object() { Json j; j.type_ = Object; return j; }

    bool is_null()   const { return type_ == Null; }
    bool is_bool()   const { return type_ == Bool; }
    bool is_int()    const { return type_ == Int; }
    bool is_number() const { return type_ == Double || type_ == Int; }
    bool is_string() const { return type_ == String; }
    bool is_array()  const { return type_ == Array; }
    bool is_object() const { return type_ == Object; }

    bool               get_bool()   const { return bval_; }
    long long          get_int()    const { return ival_; }
    double             get_double() const { return type_ == Int ? (double)ival_ : dval_; }
    const std::string& get_string() const { return sval_; }

    void push_back(const Json& v) {
        if (type_ != Array) throw std::runtime_error("not an array");
        arr_.push_back(v);
    }

    size_t size() const {
        if (type_ == Array)  return arr_.size();
        if (type_ == Object) return order_.size();
        return 0;
    }

    const Json& operator[](size_t i) const { return arr_.at(i); }
    Json&       operator[](size_t i)       { return arr_.at(i); }

    Json& operator[](const std::string& k) {
        if (type_ == Null) type_ = Object;
        if (type_ != Object) throw std::runtime_error("not an object");
        if (obj_.find(k) == obj_.end()) order_.push_back(k);
        return obj_[k];
    }
    const Json& operator[](const std::string& k) const {
        auto it = obj_.find(k);
        if (it == obj_.end()) throw std::runtime_error("key not found: " + k);
        return it->second;
    }

    bool contains(const std::string& k) const {
        return obj_.find(k) != obj_.end();
    }

    using iterator       = std::vector<Json>::iterator;
    using const_iterator = std::vector<Json>::const_iterator;
    iterator       begin()       { return arr_.begin(); }
    iterator       end()         { return arr_.end(); }
    const_iterator begin() const { return arr_.begin(); }
    const_iterator end()   const { return arr_.end(); }

    // ── serializare ───────────────────────────────────────────
    std::string dump(int indent = 2, int depth = 0) const {
        std::ostringstream os;
        std::string nl  = indent >= 0 ? "\n" : "";
        std::string sp  = indent >= 0 ? std::string((size_t)((depth+1)*indent), ' ') : "";
        std::string csp = indent >= 0 ? std::string((size_t)(depth*indent), ' ') : "";
        std::string sep = indent >= 0 ? ": " : ":";

        switch (type_) {
            case Null:   return "null";
            case Bool:   return bval_ ? "true" : "false";
            case Int:    { os << ival_; return os.str(); }
            case Double: {
                os << std::setprecision(10) << dval_;
                std::string s = os.str();
                if (s.find('.') == std::string::npos &&
                    s.find('e') == std::string::npos) s += ".0";
                return s;
            }
            case String: return escStr(sval_);
            case Array: {
                if (arr_.empty()) return "[]";
                os << "[" << nl;
                for (size_t i = 0; i < arr_.size(); i++) {
                    os << sp << arr_[i].dump(indent, depth+1);
                    if (i+1 < arr_.size()) os << ",";
                    os << nl;
                }
                os << csp << "]";
                return os.str();
            }
            case Object: {
                if (order_.empty()) return "{}";
                os << "{" << nl;
                for (size_t i = 0; i < order_.size(); i++) {
                    const std::string& k = order_[i];
                    auto it = obj_.find(k);
                    if (it == obj_.end()) continue;
                    os << sp << escStr(k) << sep
                       << it->second.dump(indent, depth+1);
                    if (i+1 < order_.size()) os << ",";
                    os << nl;
                }
                os << csp << "}";
                return os.str();
            }
        }
        return "null";
    }

    // ── parser ────────────────────────────────────────────────
    static Json parse(const std::string& src) {
        size_t pos = 0;
        return pVal(src, pos);
    }

private:
    Type type_ = Null;
    bool bval_ = false;
    long long ival_ = 0;
    double dval_ = 0.0;
    std::string sval_;
    std::vector<Json> arr_;
    std::unordered_map<std::string, Json> obj_;
    std::vector<std::string> order_;

    static std::string escStr(const std::string& s) {
        std::ostringstream os;
        os << '"';
        for (unsigned char c : s) {
            if      (c == '"')  os << "\\\"";
            else if (c == '\\') os << "\\\\";
            else if (c == '\n') os << "\\n";
            else if (c == '\r') os << "\\r";
            else if (c == '\t') os << "\\t";
            else if (c < 0x20)  os << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
            else                os << c;
        }
        os << '"';
        return os.str();
    }

    static void skipWS(const std::string& s, size_t& p) {
        while (p < s.size() && (s[p]==' '||s[p]=='\n'||s[p]=='\r'||s[p]=='\t')) p++;
    }

    static Json pVal(const std::string& s, size_t& p) {
        skipWS(s, p);
        if (p >= s.size()) throw std::runtime_error("unexpected end");
        char c = s[p];
        if (c == '"')               return pStr(s, p);
        if (c == '{')               return pObj(s, p);
        if (c == '[')               return pArr(s, p);
        if (c == 't' || c == 'f')   return pBool(s, p);
        if (c == 'n')               return pNull(s, p);
        if (c == '-' || isdigit(c)) return pNum(s, p);
        throw std::runtime_error(std::string("unexpected: ") + c);
    }

    static std::string pRawStr(const std::string& s, size_t& p) {
        p++; // skip "
        std::string r;
        while (p < s.size() && s[p] != '"') {
            if (s[p] == '\\') {
                p++;
                if      (s[p] == '"')  r += '"';
                else if (s[p] == '\\') r += '\\';
                else if (s[p] == '/')  r += '/';
                else if (s[p] == 'n')  r += '\n';
                else if (s[p] == 'r')  r += '\r';
                else if (s[p] == 't')  r += '\t';
                else if (s[p] == 'u') {
                    p++;
                    int cp = std::stoi(s.substr(p,4), nullptr, 16);
                    p += 3;
                    if (cp < 0x80) r += (char)cp;
                    else if (cp < 0x800) { r += (char)(0xC0|(cp>>6)); r += (char)(0x80|(cp&0x3F)); }
                    else { r += (char)(0xE0|(cp>>12)); r += (char)(0x80|((cp>>6)&0x3F)); r += (char)(0x80|(cp&0x3F)); }
                } else r += s[p];
            } else r += s[p];
            p++;
        }
        p++; // skip "
        return r;
    }

    static Json pStr(const std::string& s, size_t& p)  { return Json(pRawStr(s,p)); }

    static Json pBool(const std::string& s, size_t& p) {
        if (s.substr(p,4)=="true")  { p+=4; return Json(true); }
        if (s.substr(p,5)=="false") { p+=5; return Json(false); }
        throw std::runtime_error("invalid bool");
    }

    static Json pNull(const std::string& s, size_t& p) {
        if (s.substr(p,4)=="null") { p+=4; return Json(); }
        throw std::runtime_error("invalid null");
    }

    static Json pNum(const std::string& s, size_t& p) {
        size_t start = p;
        bool flt = false;
        if (s[p]=='-') p++;
        while (p<s.size() && isdigit(s[p])) p++;
        if (p<s.size() && s[p]=='.') { flt=true; p++; }
        while (p<s.size() && isdigit(s[p])) p++;
        if (p<s.size() && (s[p]=='e'||s[p]=='E')) {
            flt=true; p++;
            if (p<s.size() && (s[p]=='+'||s[p]=='-')) p++;
            while (p<s.size() && isdigit(s[p])) p++;
        }
        std::string num = s.substr(start, p-start);
        if (flt) return Json(std::stod(num));
        return Json((long long)std::stoll(num));
    }

    static Json pArr(const std::string& s, size_t& p) {
        p++;
        Json a = Json::array();
        skipWS(s,p);
        if (p<s.size() && s[p]==']') { p++; return a; }
        while (true) {
            a.push_back(pVal(s,p));
            skipWS(s,p);
            if (p>=s.size()) break;
            if (s[p]==']') { p++; break; }
            if (s[p]==',') p++;
        }
        return a;
    }

    static Json pObj(const std::string& s, size_t& p) {
        p++;
        Json o = Json::object();
        skipWS(s,p);
        if (p<s.size() && s[p]=='}') { p++; return o; }
        while (true) {
            skipWS(s,p);
            std::string key = pRawStr(s,p);
            skipWS(s,p);
            if (p>=s.size()||s[p]!=':') throw std::runtime_error("expected ':'");
            p++;
            o[key] = pVal(s,p);
            skipWS(s,p);
            if (p>=s.size()) break;
            if (s[p]=='}') { p++; break; }
            if (s[p]==',') p++;
        }
        return o;
    }
};
