#include <iostream>
#include <stdexcept>
#include <fstream>
#include <gaussian_input.h>
#include <parameter_group.h>
#include <header.h>
#include <gaussian.h>
#include <chrono>
#include <gaussian_output.h>

using namespace std;
using namespace reparm;
using namespace chrono;

int main(){

  ifstream fin{"FuranDFT.log"};
  string outfile;
  string line;
  while (getline(fin, line)){
    outfile += line + "\n";
  }

  GaussianOutput output(outfile);
  cout << output.str();


  return 0;

}
