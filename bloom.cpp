#include "bloom.h"
#include "CLI11.hpp"
//#include <CLI/StringTools.hpp>
#include <iostream>

int main(int argc, char **argv) {

  CLI::App app{"My Program"};

  // std::string arg1, arg2;
  // app.add_option("arg1", arg1, "The first argument")->required();
  // app.add_option("arg2", arg2, "The second argument")->required();

  std::string input = "words.txt";
  std::string s;
  long long int m = 1000000;
  int k = 2;
  app.add_option("-d, --dict", s, "Testing file");

  app.add_option("-m, --bits", m, "Allocating the size of the vector");

  app.add_option("-k, --hash", k, "Hashes to try");

  CLI11_PARSE(app, argc, argv);
  if (k < 2 || k > 3) {
    k = 2;
  }

  BloomFilter bf(m, k, s);
  bf.output();

  return 0;
}
