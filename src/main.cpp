#include <iostream>
#include <stdexcept>
#include <fstream>
#include <gaussian_input.h>
#include <parameter_group.h>
#include <header.h>
#include <gaussian.h>
#include <chrono>

using namespace std;
using namespace reparm;
using namespace chrono;

int main(){

  vector<GaussianInput> inputs;
  for (int i = 0; i < 100; ++i){
    GaussianInput gin("FuranAM1.com");
    inputs.push_back(gin);
  }

  ParameterGroup param_group(inputs);
  param_group.Mutate(0.1, 0.5);
  Gaussian gaussian{param_group};

  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  vector<string> s = gaussian.RunGaussian();
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
  cout << time_span.count() << endl;




  return 0;

}
