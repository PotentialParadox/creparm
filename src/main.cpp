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
#include <breed.h>
#include <aristocratic_cloning.h>
#include <print_best.h>

using namespace std;
using namespace reparm;
using namespace chrono;


int main(){
  // Read the input files and convert to a Reparm Gaussian Input
  ReparmData reparm_data{"reparm.in"};
  try{
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
    Breed breed(reparm_data);
    AristocraticCloning aristocratic_clone(reparm_data, fitness);

    mutate(reparm_data.population_, 0, reparm_data.GetReparmInput().GetPopulationSize());

    // ******* Begin the main loop *********
    for (int i = 0; i < reparm_data.GetReparmInput().GetNumberGenerations(); ++i){
      survivor(reparm_data.population_);
      aristocratic_clone(reparm_data.population_);
      breed(reparm_data.population_);
      mutate(reparm_data.population_);

      cout << "Fitness of Step " << i <<  endl;
      cout << fitness.StringList(reparm_data.population_[0]);
    }


  }
  catch(const char *e){
    cerr << e << endl;
  }
  cout << reparm_data.population_[0].GetFitness() << endl;
  PrintBest print_best{reparm_data};
  print_best();
  
  return 0;
}
