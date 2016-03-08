#include <iostream>
#include <stdexcept>
#include <fstream>
#include <gaussian_input.h>
#include <parameter_group.h>
#include <header.h>
#include <gaussian.h>
#include <chrono>
#include <gaussian_output.h>
#include <genetic_algorithm.h>
#include <reparm_data.h>
#include <reparm_input.h>

using namespace std;
using namespace reparm;
using namespace chrono;


int main(){
  // Read the input files and convert to a Reparm Gaussian Input
  ReparmData reparm_data{"reparm.in"};
  ReparmInput reparm_input{reparm_data.GetReparmInput()};
  string starter_file{reparm_input.GetMoleculeName() + ".com"};
  GaussianInput input{CreateReparmGaussian(starter_file)};

  // Create the AM1 population from this formated input
  reparm_data.CreatePopulation(input);

  // Calculate the high level outputs
  reparm_data.CalculateHighLevel();


  
  
  return 0;
}
