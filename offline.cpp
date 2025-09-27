#include <iostream>
#include <iomanip>
#include <chrono>
#include "common.hpp"

void readSamples(const std::string& filename, TupleList& F, TupleList& T) {
  SampleStream stream(filename);
  while (auto sample = stream.next()) {
    switch (sample->type) {
      case NEGATIVE: {
        F.push_back(sample->tuple);
      }
        break;
      case POSITIVE: {
        T.push_back(sample->tuple);
      }
        break;
    }
  }
}

Formula learn_horn_offline(const TupleList& F, const TupleList& T) {
  Formula formula;
  for (const auto& f : F) {
    Clause clause = new_clause(f, T);
    formula.push_back(clause);
    if (clause.pivot == arity) {
      throw std::runtime_error("problem unsolvable");
    }
  }
  return formula;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " inputfile" << std::endl;
    return 1;
  }
  std::string filename = argv[1];
  
  TupleList F;
  TupleList T;
  readSamples(filename, F, T);
   
  try {
    auto start = std::chrono::high_resolution_clock::now();
    Formula formula = learn_horn_offline(F, T);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << to_string(formula);
    std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cerr << std::setw(13) << std::left << filename << ": "
              << std::fixed << std::setprecision(3) <<std::setw(6) << std::right
              << elapsed.count() << " seconds" << std::endl;
    return 0;
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return 2;
  }
}
