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
  ofstream fout{"reparm.out"};
  try{
    ReparmInput reparm_input{reparm_data.GetReparmInput()};
    string starter_file{reparm_input.GetMoleculeName() + ".com"};
    GaussianInput input{CreateReparmGaussian(starter_file)};

    fout << "Creating AM1 population" << endl;
    reparm_data.CreatePopulation(input);
    // Since the entire population is the same, we only run the first
    Gaussian gaussian{reparm_data.population_[0]};
    vector<GaussianOutput> gouts{gaussian.RunGaussian()};
    reparm_data.population_[0].SetOutputs(gouts);
    initial_output = reparm_data.population_[0].GetOutputs()[0].str();

    fout << "Calculating High Level Theory" << endl;
    fout << "This could take a lot of time" << endl;
    reparm_data.CalculateHighLevel();
    fout << "Finished with High Level Theory" << endl;

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
    double best_fitness = 1;
    for (int i = 0; i < reparm_data.GetReparmInput().GetNumberGenerations(); ++i){

      fout << "Step: " << i << endl;
      survivor(reparm_data.population_);
      aristocratic_clone(reparm_data.population_);
      breed(reparm_data.population_);
      mutate(reparm_data.population_);

      if (best_fitness > reparm_data.population_[0].GetFitness()){
        fout << "New Best Fitness Found at Step " << i <<  endl;
        fout << fitness.StringList(reparm_data.population_[0]) << endl;
        best_fitness = reparm_data.population_[0].GetFitness();
      }
    }
  }
  catch(const char *e){
    fout << "An Error was Found" << endl;
    fout << e << endl;
  }
  // Create a function for this
  reparm_data.RunBest();
  fout << "Initial Output" << endl;
  fout << initial_output << endl;
  fout << "BEST OUTPUTS" << endl;
  fout << reparm_data.population_[0].GetOutputs()[0].str() << endl;
  fout << "Corresponding DFT" << endl;
  fout << reparm_data.high_level_outputs_[0].str() << endl;
  fout << "Fitness: " << reparm_data.population_[0].GetFitness() << endl;
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
  int time = static_cast<int>(time_span.count());
  fout << "Job took " << static_cast<int>(time/3600) << " hours ";
  fout << static_cast<int>( (time % 3600) / 60 ) << " minutes ";
  fout << static_cast<int>(time % 60) << " seconds";
  fout.close();
}
