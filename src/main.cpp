#include <iostream>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <gaussian_input.h>
#include <parameter_group.h>
#include <header.h>
#include <gaussian.h>
#include <chrono>
#include <gaussian_output.h>
#include <genetic_algorithm.h>
#include <reparm_data.h>
#include <reparm_input.h>
#include <fitness.h>
#include <mutate.h>
#include <survivor.h>

using namespace std;
using namespace reparm;
using namespace chrono;


int main(){
  try{
    // Read the input files and convert to a Reparm Gaussian Input
    ReparmData reparm_data{"reparm.in"};
    ReparmInput reparm_input{reparm_data.GetReparmInput()};
    string starter_file{reparm_input.GetMoleculeName() + ".com"};
    GaussianInput input{CreateReparmGaussian(starter_file)};

    // Create the AM1 population from this formated input
    reparm_data.CreatePopulation(input);
    // Since the entire population is the same, we only run the first
    Gaussian gaussian{reparm_data.population_[0]};
    vector<GaussianOutput> gouts{gaussian.RunGaussian()};
    reparm_data.population_[0].SetOutputs(gouts);

    // Calculate the high level outputs
    reparm_data.CalculateHighLevel();

    // Initialize the functors
    Fitness fitness(reparm_data.population_[0], reparm_data.GetHighLevelOutputs());
    Mutate mutate(reparm_data, fitness);
    Survivor survivor(reparm_data);

    // ******* Begin the main loop *********
    mutate(reparm_data.population_, 0, 6);
    for (auto &i: reparm_data.population_){cout << i.GetFitness() << " ";}
    cout << endl;
    survivor(reparm_data.population_);
    for (auto &i: reparm_data.population_){cout << i.GetFitness() << " ";}
    cout << endl;
  }
  catch(const char *e){
    cerr << e << endl;
  }


  
  
  return 0;
}
