#include <sstream>
#include <limits>
#include "common.hpp"

const size_t UNDEF = std::numeric_limits<size_t>::max();

size_t arity = UNDEF; // Tuple length

std::string to_string(const Tuple& t) {
  std::ostringstream oss;
  oss << "(";
  for (size_t i = 0; i < t.size(); ++i) {
    if (i > 0)
      oss << ",";
    oss << t[i];
  }
  oss << ")";
  return oss.str();
}

Tuple tuple_min(const Tuple& a, const Tuple& b) {
  Tuple c(arity);
  for (size_t i = 0; i < a.size(); ++i)
    c[i] = std::min(a[i], b[i]);
  return c;
}

std::string to_string(const Clause& clause) {
  std::ostringstream oss;
  oss << "(";
  std::string sep = "";
  if (clause.pivot < arity && clause.minTf[clause.pivot] != UNDEF) { 
    oss << "x" << std::to_string(clause.pivot) << ">=" << std::to_string(clause.f[clause.pivot]+1);
    sep = " + ";
  }
  for (size_t i = 0; i < clause.f.size(); ++i) {
    if (clause.f[i] > 0) {
      oss << sep << "x" << std::to_string(i) << "<=" << std::to_string(clause.f[i]-1);
      sep = " + ";
    }
  }
  oss << ")";
  return oss.str();
}

std::string to_string(const Formula& formula) {
  std::ostringstream oss;
  std::string sep = "\t  ";
  for (const Clause &clause : formula) {
    oss << sep << to_string(clause) << std::endl;
    sep = "\t* ";
  }
  return oss.str();
}

Tuple min(const TupleList& T, const Tuple& f) {
  Tuple m(arity, UNDEF);
  for (const Tuple& t : T)
    if (f <= t)
      m = tuple_min(m, t);
  return m;
}

// Create clause from tuple f and current T
Clause new_clause(const Tuple& f, const TupleList& T) {
  Clause clause;
  clause.f = f;
  clause.minTf = min(T,f);
  size_t i = 0;
  while (i < clause.minTf.size() && clause.minTf[i] != UNDEF && clause.minTf[i] == f[i]) {
    ++i;
  }
  clause.pivot = i;
  return clause;
}

// Update clause when adding t to T
void update_clause(const Tuple& t, Clause& clause) {
  clause.minTf = tuple_min(t, clause.minTf);
  size_t i = clause.pivot;
  while (i < clause.minTf.size() && clause.minTf[i] == clause.f[i])
    ++i;
  clause.pivot = i;
}

SampleStream::SampleStream(const std::string& filename)
  : file(filename), lineno(0) {
  if (!file.is_open())
    throw std::runtime_error("Could not open file: " + filename);
}

std::unique_ptr<Sample> SampleStream::next() {
  std::string line;
  while (std::getline(file, line)) {
    ++lineno;
    std::istringstream ss(line);
    std::string token;
    Tuple values;
    while (std::getline(ss, token, ',')) {
      try {
        values.push_back(std::stoul(token));
      } catch (...) {
        throw std::runtime_error("Input line " + std::to_string(lineno) + ": not an unsigned int");
      }
    }
    if (values.empty() || (values[0] != NEGATIVE && values[0] != POSITIVE))
      throw std::runtime_error("Input line " + std::to_string(lineno) + ": invalid sample type");
    if (values.size() < 2)
      throw std::runtime_error("Input line " + std::to_string(lineno) + ": sample length must be positive");
    if (arity == UNDEF)
      arity = values.size() - 1;
    if (values.size() != arity + 1)
      throw std::runtime_error("Input line " + std::to_string(lineno) + ": inconsistent sample length");
    if (valuesSeen.insert(values).second) {
      unsigned short type = values[0];
      auto tuple = Tuple(values.begin() + 1, values.end());
      return std::make_unique<Sample>(Sample{type, tuple});
    }
  }
  return nullptr;
}
