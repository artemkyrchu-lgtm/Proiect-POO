#include "Carte.h"

Carte::Carte(std::string t, std::vector<std::string> aut, std::string i, int v, int p,
             std::string det, double rat, int rev, std::string ser)
    : titlu(t), autori(aut), isbn(i), volum(v), parte(p),
      detalii(det), rating(rat), editieRevizie(rev), serie(ser) {}
