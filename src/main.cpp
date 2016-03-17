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

using namespace std;
using namespace reparm;
using namespace chrono;


int main(){
  std::string initial_output;
  // Read the input files and convert to a Reparm Gaussian Input
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
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
    initial_output = reparm_data.population_[0].GetOutputs()[0].str();

    // Calculate the high level outputs
    reparm_data.CalculateHighLevel();

    // Initialize the functors
    Fitness fitness(reparm_data.population_[0], reparm_data.GetHighLevelOutputs());
    Mutate mutate(reparm_data, fitness);
    Survivor survivor(reparm_data);
    Breed breed(reparm_data);
    AristocraticCloning aristocratic_clone(reparm_data, fitness);

    // Index starts at one, guarenteeing that at least member is at least as good as
    // the user's input
    mutate(reparm_data.population_, 1, reparm_data.GetReparmInput().GetPopulationSize());

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
  reparm_data.RunBest();
  cout << "Initial Output" << endl;
  cout << initial_output << endl;
  cout << "BEST OUTPUTS" << endl;
  cout << reparm_data.population_[0].GetOutputs()[0].str() << endl;
  cout << "Corresponding DFT" << endl;
  cout << reparm_data.high_level_outputs_[0].str() << endl;
  cout << "Fitness: " << reparm_data.population_[0].GetFitness() << endl;
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
  cout << "Job took " << time_span.count() << " seconds" << endl;
  
  return 0;
}
