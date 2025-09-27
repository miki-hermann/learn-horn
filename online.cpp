#include <iostream>
#include <iomanip>
#include <chrono>
#include "common.hpp"

void readSamples(const std::string& filename, SampleList& samples) {
  SampleStream stream(filename);
  while (auto sample = stream.next())
    samples.push_back(*sample);
}

Formula learn_horn_online(SampleList samples) {
  TupleList T;
  Formula formula;
  for (const auto& sample : samples) {
    switch (sample.type) {
      case NEGATIVE: {
        Clause clause = new_clause(sample.tuple, T);
        formula.push_back(clause);
        if (clause.pivot == arity)
          throw std::runtime_error("problem unsolvable");
      }
        break;
      case POSITIVE: {
        T.push_back(sample.tuple);
        for (auto& clause : formula) {
          if (clause.f <= sample.tuple) {
            update_clause(sample.tuple, clause);
            if (clause.pivot == arity)
              throw std::runtime_error("problem unsolvable");
          }
        }
      }
        break;
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

  SampleList samples;
  readSamples(filename, samples);

  try {
    auto start = std::chrono::high_resolution_clock::now();
    Formula formula = learn_horn_online(samples);
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
