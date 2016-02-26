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

  vector<GaussianInput> inputs;
  for (int i=0; i<4; ++i){
    GaussianInput input("FuranDFT.com");
    GaussianInput freq("FuranDFTfreq.com");
    input.Link(freq);
    inputs.push_back(input);
  }
  ParameterGroup param_group{inputs};
  Gaussian gaussian{param_group};
  vector<GaussianOutput> outputs;
  outputs = gaussian.RunGaussian();
  cout << outputs[0].str() << endl;

  return 0;

}
