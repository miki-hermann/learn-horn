#include <vector>
#include <string>
#include <unordered_set>
#include <fstream>
#include <memory>

const unsigned short NEGATIVE = 0;
const unsigned short POSITIVE = 1;

extern size_t arity; // Tuple length

using Tuple = std::vector<size_t>;
using TupleList = std::vector<Tuple>;

// Hash and equality for Tuple for use in unordered_set
struct TupleHash {
  size_t operator()(const Tuple& t) const {
    size_t h = 0;
    for (const size_t& x : t)
      h ^= std::hash<size_t>{}(x) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
  }
};

struct TupleEqual {
  bool operator()(const Tuple& a, const Tuple& b) const {
    return a == b;
  }
};

using TupleSet = std::unordered_set<Tuple, TupleHash, TupleEqual>;

struct Sample {
  unsigned short type;
  Tuple tuple;
};
using SampleList = std::vector<Sample>;

class SampleStream {
public:
  explicit SampleStream (const std::string& filename);
  std::unique_ptr<Sample> next();
private:
  std::ifstream file;
  TupleSet valuesSeen;
  size_t lineno;
};

// Clause structure
struct Clause {
  Tuple f;
  Tuple minTf;
  size_t pivot;
};
using Formula = std::vector<Clause>;

inline bool operator <=(const Tuple& a, const Tuple& b) {
  for (size_t i = 0; i < arity; ++i)
    if (a[i] > b[i])
      return false;
  return true;
}

Tuple tuple_min(const Tuple& a, const Tuple& b);
Tuple min(const TupleList& T, const Tuple& f);
std::string to_string(const Tuple& t);
std::string to_string(const Clause& clause);
std::string to_string(const Formula& formula);
// Create clause from tuple f and current T
Clause new_clause(const Tuple& f, const TupleList& T);
// Update clause when adding t to T
void update_clause(const Tuple& t, Clause& clause);
