#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

const char delim = ',';
ifstream infile;
ofstream outfile;

vector<int> split (const string &strg, char delimiter) {
  // splits a string into chunks separated by delimiter (split in perl)
  vector<int> chunks;
  string token;
  istringstream iss(strg);

  while (getline(iss, token, delimiter))
    chunks.push_back(stoi(token));
  return chunks;
}

int main (int argc, char **argv) {
  system("unxz --keep --force digit.csv.xz");
  cerr << "Which digit (0-9) do you want to recognize? ";
  int digit;
  cin >> digit;
  cerr <<  "Recognizing digit " << digit << endl;

  infile.open("digit.csv");
  if (! infile.is_open()) {
    cerr << "+++ Cannot open input file digit.csv" << endl;
    exit(1);
  }

  string filename = "digit" + to_string(digit) + ".csv";
  outfile.open(filename);
  if (! outfile.is_open()) {
    cerr << "+++ Cannot open output file " << filename << endl;
    exit(1);
  }

  string line;
  while (getline(infile, line)) {
    vector<int> digits = split(line, delim);
    int leader = digits[0] == digit ? 1 : 0;
    outfile << leader << ',';
    outfile << line << endl;
  }

  infile.close();
  outfile.close();
  system("rm -f digit.csv");
  cerr << "CSV file " << filename << " generated" << endl;
}

////////////////////////////////////////////////////////////////////////////////
