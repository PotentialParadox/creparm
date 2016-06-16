#include <parameter_group.h>
#include "mutate.h"
#include <iostream>
#include <algorithm>
#include <gaussian.h>

void reparm::Mutate::operator()(std::vector<reparm::ParameterGroup> &population){
  float mutation_perturbation = reparm_data_->GetReparmInput().GetMutationPerturbations();
  float mutation_rate = reparm_data_->GetReparmInput().GetMutationRate();
  int number_elites = reparm_data_->GetReparmInput().GetNumberElites();
  sort(population.begin(), population.end());
  auto it = population.begin() + number_elites;
  Gaussian gaussian;
  for (; it < population.end(); ++it){
    it->Mutate(mutation_perturbation, mutation_rate);
    auto backup = *it;
    std::vector<reparm::GaussianOutput> gouts;
    try{
      gouts = gaussian.RunGaussian(*it);
    }
    catch(...){
      std::cout << "Gaussian failed after mutation" << std::endl;
      *it = backup;
    }
    it->SetOutputs(gouts);
  }
}

void reparm::Mutate::operator()(std::vector<reparm::ParameterGroup> &population,
                                unsigned int start, unsigned int finish){
  float mutation_perturbation = reparm_data_->GetReparmInput().GetMutationPerturbations();
  float mutation_rate = reparm_data_->GetReparmInput().GetMutationRate();
  if (start > population.size() || start > finish || finish > population.size())
    throw "Error with mutate indices\n";
  auto it = population.begin() + start;
  auto end = population.begin() + finish;
  Gaussian gaussian;
  for (; it < end; ++it){
    it->Mutate(mutation_perturbation, mutation_rate);
    auto backup = *it;
    std::vector<reparm::GaussianOutput> gouts;
    try{
      gouts = gaussian.RunGaussian(*it);
    }
    catch(...){
      std::cout << "Gaussian failed after mutation" << std::endl;
      *it = backup;
    }
    it->SetOutputs(gouts);
  }
}

