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
#include <genesis.h>
#include <gout_reader.h>
#include <container_math.h>
#include <diversity.h>

using namespace std;
using namespace reparm;
using namespace chrono;


int main(){

  std::string initial_output;

  /* Read the input files and convert to a Reparm Gaussian Input */
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  
  shared_ptr<ReparmData> reparm_data(make_shared<ReparmData>("reparm.in"));
  ofstream fout{"reparm.out"};

  fout << PrintTitle();

  GaussianInput input;

  double original_fitness = 0;
  try{

    ReparmInput reparm_input{reparm_data->GetReparmInput()};
    if (reparm_input.GetShouldContinue()){
      fout << "Continuing from last run" << endl;
      reparm_data->Load();
    }
    else{
      fout << "Starting new job" << endl;
      Genesis genesis(reparm_data);
      fout << "Saved structures to reparm_structures.xyz" << endl;
      ofstream xyz{"reparm_structures.xyz"};
      xyz << reparm_data->population_[0].XYZString();
    }

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
    original_fitness = fitness(reparm_data->population_[0]);
    reparm_data->SetOriginalFitness(original_fitness);
    reparm_data->Save();
    double best_fitness = original_fitness;
    fout << "\nOriginal Fitness" << endl;
    fout << fitness.StringList(reparm_data->population_[0]) << endl;
    int steps_since_last_best = 0;
    for (int i = 0; i < reparm_data->GetReparmInput().GetNumberGenerations(); ++i){
      fout << "Step: " << i << endl;
      survivor(reparm_data->population_);
      aristocratic_clone(reparm_data->population_, fitness);
      fitness(reparm_data->population_);
      breed(reparm_data->population_);
      mutate(reparm_data->population_);
      fitness(reparm_data->population_);

      if (best_fitness > reparm_data->population_[0].GetFitness()){
	steps_since_last_best = 0;
        fout << "New Best Fitness Found at Step " << i <<  endl;
        fout << fitness.StringList(reparm_data->population_[0]);
        best_fitness = reparm_data->population_[0].GetFitness();
    	  fout << "Total Fitness: " << best_fitness / original_fitness << "\n" << endl;
    	  reparm_data->Save();
      }
      else ++steps_since_last_best;
      /* If struggling to improve, adjust the population size and
	 mutation rates first, else call convergence. */
      if (!reparm_data->Adjust(steps_since_last_best))
	  break;
    }
  }
  catch(const char *e){
    fout << "An Error was Found" << endl;
    fout << e << endl;
  }
  /* Create a function for this */
  reparm_data->RunBest();
  fout << "Initial Output" << endl;
  fout << initial_output << endl;
  fout << "BEST OUTPUTS" << endl;
  fout << reparm_data->population_[0].GetOutputs()[0].str() << endl;
  Diversity diversity(reparm_data);
  diversity.Sort();
  fout << "Corresponding DFT" << endl;
  fout << reparm_data->high_level_outputs_[0].str() << endl;
  fout << "Fitness: " << reparm_data->population_[0].GetFitness()
    / original_fitness << endl;
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
  int time = static_cast<int>(time_span.count());
  fout << "Job took " << static_cast<int>(time/3600) << " hours ";
  fout << static_cast<int>( (time % 3600) / 60 ) << " minutes ";
  fout << static_cast<int>(time % 60) << " seconds";
  fout.close();
  return 0;
}
