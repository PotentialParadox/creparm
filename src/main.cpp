#include <iostream>
#include <stdexcept>
#include <fstream>
#include <gaussian_input.h>
#include <parameter_group.h>
#include <header.h>
#include <gaussian.h>

using namespace std;
using namespace reparm;

int main(){

  vector<GaussianInput> inputs;
  for (int i = 0; i < 2; ++i){
    GaussianInput gin("FuranAM1.com");
    inputs.push_back(gin);
  }

  ParameterGroup param_group(inputs);
  param_group.Mutate(0.1, 0.5);
  Gaussian gaussian{param_group};

  vector<string> s = gaussian.RunGaussian();
  //for (auto i: s) {cout << i << endl;}



  return 0;

}
