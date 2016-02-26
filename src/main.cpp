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
    for (int i=0; i<12; ++i){
      GaussianInput input("FuranDFT.com");
      input.PerturbCoordinates(0.5);
      GaussianInput freq(input);
      Header header("#P B3LYP/3-21G freq\n\nHi\n");
      freq.SetHeader(header);
      input.Link(freq);
      inputs.push_back(input);
    }
    ParameterGroup param_group{inputs};
    Gaussian gaussian{param_group};
    vector<GaussianOutput> outputs;
    outputs = gaussian.RunGaussian();
    cout << outputs[0].str() << endl;
  }
  catch(...){
    cerr << "Caught Gaussian Error" << endl;
  }

  return 0;

}
