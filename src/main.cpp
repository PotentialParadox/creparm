#include <iostream>
#include <stdexcept>
#include <memory>
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
  auto reparm_data = unique_ptr<ReparmData>(new ReparmData("reparm.in"));
  ofstream fout{"reparm.out"};
  fout << PrintTitle();
  GaussianInput input;
  try{
    ReparmInput reparm_input{reparm_data->GetReparmInput()};
    if (reparm_input.GetShouldContinue()){
      fout << "Continuing from last run" << endl;
      input = GaussianInput{"best_eq.com"};
      Header header{"#P AM1(Input,print)\n"};
      input.SetHeader(header);
    }
    else{
      string starter_file{reparm_input.GetMoleculeName() + ".com"};
      input = GaussianInput{CreateReparmGaussian(starter_file)};
    }
    fout << "Creating AM1 population" << endl;
    reparm_data->CreatePopulation(input);
    // Since the entire population is the same, we only run the first
    Gaussian gaussian{reparm_data->population_[0]};
    vector<GaussianOutput> gouts{gaussian.RunGaussian()};
    reparm_data->population_[0].SetOutputs(gouts);
    initial_output = reparm_data->population_[0].GetOutputs()[0].str();

    fout << "Calculating High Level Theory" << endl;
    fout << "This could take a lot of time" << endl;
    reparm_data->CalculateHighLevel();
    fout << "Finished with High Level Theory" << endl;

    // Initialize the functors
    Mutate mutate(reparm_data);
    fout << "Initialized mutate" << endl;
    initial_output = reparm_data->population_[0].GetOutputs()[0].str();
    Survivor survivor(reparm_data);
    fout << "Initialized survivor" << endl;
    initial_output = reparm_data->population_[0].GetOutputs()[0].str();
    Breed breed(reparm_data);
    fout << "Initialized breed" << endl;
    initial_output = reparm_data->population_[0].GetOutputs()[0].str();
    AristocraticCloning aristocratic_clone(reparm_data);
    fout << "Initialized ac" << endl;
    initial_output = reparm_data->population_[0].GetOutputs()[0].str();

    // Index starts at one, guarenteeing that at least member is at least as good as
    // the user's input
    mutate(reparm_data->population_, 1, reparm_data->GetReparmInput().GetPopulationSize());
    fout << "Finished first mutation" << endl;
    initial_output = reparm_data->population_[0].GetOutputs()[0].str();

    fout << "Determining fitnesss" << endl;
    Fitness fitness(reparm_data->population_, reparm_data->GetHighLevelOutputs());
    fout << "Determined fitnesss" << endl;
    initial_output = reparm_data->population_[0].GetOutputs()[0].str();

    // ******* Begin the main loop *********
    double original_fitness = fitness(reparm_data->population_[0]);
    double best_fitness = original_fitness;
    fout << "\nOriginal Fitness" << endl;
    fout << fitness.StringList(reparm_data->population_[0]) << endl;
    for (int i = 0; i < reparm_data->GetReparmInput().GetNumberGenerations(); ++i){

      fout << "Step: " << i << endl;
      survivor(reparm_data->population_);
      aristocratic_clone(reparm_data->population_, fitness);
      fitness(reparm_data->population_);
      breed(reparm_data->population_);
      mutate(reparm_data->population_);
      fitness(reparm_data->population_);

      if (best_fitness > reparm_data->population_[0].GetFitness()){
        fout << "New Best Fitness Found at Step " << i <<  endl;
        fout << fitness.StringList(reparm_data->population_[0]);
        best_fitness = reparm_data->population_[0].GetFitness();
	fout << "Total Fitness: " << best_fitness / original_fitness << "\n" << endl;
      }
    }
  }
  catch(const char *e){
    fout << "An Error was Found" << endl;
    fout << e << endl;
  }
  // Create a function for this
  reparm_data->RunBest();
  fout << "Initial Output" << endl;
  fout << initial_output << endl;
  fout << "BEST OUTPUTS" << endl;
  fout << reparm_data->population_[0].GetOutputs()[0].str() << endl;
  fout << "Corresponding DFT" << endl;
  fout << reparm_data->high_level_outputs_[0].str() << endl;
  fout << "Fitness: " << reparm_data->population_[0].GetFitness() << endl;
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
  int time = static_cast<int>(time_span.count());
  fout << "Job took " << static_cast<int>(time/3600) << " hours ";
  fout << static_cast<int>( (time % 3600) / 60 ) << " minutes ";
  fout << static_cast<int>(time % 60) << " seconds";
  fout.close();
}
