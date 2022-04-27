#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include "matrix+formula.hpp"
#include "common.hpp"

using namespace std;

namespace std {
  template<> struct hash<Clause> {
    using argument_type = Clause;
    using result_type = size_t;

    result_type operator() (const argument_type &clause) const {
      return hash<string>()(to_string(clause));
    }
  };
}

unordered_map<Clause, int> pivot;
unordered_map<Clause, Row> origin;
map<Row, Row> guard;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void learn_negative (Row &f, Formula &varphi) {
  negativeF.insert(f);
  guard[f] = Row(arity, DCARD);
  for (Row t : positiveT)
    if (t >= f)
      guard[f] = min(t, guard[f]);
  if (guard[f] == f) {
    cerr << "+++ negative example present in Horn closure of T" << endl;
    throw;
  }
  Clause c;
  for (int i = 0; i < arity; ++i) {
    Sign sign = f[i] > 0 ? lneg : lnone;
    unsigned val = f[i] > 0 ? f[i]-1 : 0;
    Literal lit(sign, 0, val);
    c.push_back(lit);
  }
  if (positiveT |= c) {
    varphi.insert(c);
    pivot[c] = -1;
    origin[c] = f;
  } else {
    bool eliminated = false;
    int i = 0;
    Clause newc;
    while (!eliminated) {
      if (f[i] < DCARD-1) {
	newc = c;
	newc[i].sign = (Sign) (newc[i].sign | lpos);
	newc[i].pval = f[i]+1;
	if (positiveT |= newc) {
	  varphi.insert(newc);
	  pivot[newc] = i;
	  eliminated = true;
	}
      }
      i++;
    }
    origin[newc] = f;
  }
}

void learn_positive (Row &t, Formula &varphi) {
  positiveT.insert(t);
  for (Row f : negativeF)
    if (f >= t) {
      guard[f] = min(t, guard[f]);
      if (guard[f] == f) {
	cerr << "+++ negative example present in Horn closure of T" << endl;
	throw;
      }
    }
  const Matrix X = {t};
  for (Clause c : varphi) {
    if (! (X |= c)) {
      Row f = origin[c];
      int i = pivot[c];
      if (i >= 0) {
	c[i].sign = (Sign) (c[i].sign ^ lpos);
	c[i].pval = 0;
      }
      bool eliminated = false;
      while (! eliminated) {
	i++;
	if (f[i] < DCARD-1) {
	  Clause newc = c;
	  newc[i].sign = (Sign) (c[i].sign | lpos);;
	  newc[i].pval = f[i]+1;
	  if (positiveT |= newc) {
	    c = newc;
	    pivot[c] = i;
	    eliminated = true;
	  }
	}
      }
    }
  }
}

Formula learn_horn_online () {
  Formula varphi;
  string line;
  while (getline(cin, line)) {
    Row m = split(line, delim);
    unsigned example = m.front();
    m.pop_front();
    if (arity == 0)
      arity = m.size();
    if (example > 1) {
      cerr << "+++ '" << line << "'" << endl;
      cerr << "+++ leading indicator not 0 or 1; example dropped" << endl;
    } else if (arity != m.size()) {
      cerr << "+++ '" << line << "'" << endl;
      cerr << "+++ arity discrepancy; example dropped" << endl;
    } else if (example == 0)
      learn_negative(m, varphi);
    else if (example == 1)
      learn_positive(m, varphi);
  }
  return varphi;
}

int main (int argc, char **argv) {
  read_arg(argc, argv);
  string command = argv[0];
  adjust_and_open(command);
  Formula varphi = learn_horn_online();
  cout << to_string(varphi) << endl;
  if (!latex.empty())
    latexfile << to_latex(varphi) << endl;
}

////////////////////////////////////////////////////////////////////////////////
