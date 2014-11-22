#include "formule.hpp"
#include "parser.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv) {
  formule f;
  if (argc <= 1)
    f = lit_formule(cin);
  else
    f = lit_formule(string(argv[1]));
    
  cout << f << endl;
  free_all(f);
  return EXIT_SUCCESS;
}
