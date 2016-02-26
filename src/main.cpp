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

  try{
    vector<GaussianInput> inputs;
    for (int i=0; i<40; ++i){
      GaussianInput input("FuranAM1.com");
      input.PerturbCoordinates(1.5);
      inputs.push_back(input);
    }
    ParameterGroup param_group{inputs};
    Gaussian gaussian{param_group};
    vector<GaussianOutput> outputs;
    outputs = gaussian.RunGaussian();
  }
  catch(...){
    cerr << "Caught Gaussian Error" << endl;
  }

  return 0;

}
