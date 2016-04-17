#include <mutate.h>
#include <parameter_group.h>
#include <iostream>
#include <algorithm>
#include <gaussian.h>

void reparm::Mutate::operator()(std::vector<reparm::ParameterGroup> &population){
  sort(population.begin(), population.end());
  auto it = population.begin() + number_elites_;
  Gaussian gaussian;
  for (; it < population.end(); ++it){
    it->Mutate(mutation_perturbation_, mutation_rate_);
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
  if (start > population.size() || start > finish || finish > population.size())
    throw "Error with mutate indices\n";
  sort(population.begin(), population.end());
  auto it = population.begin() + start;
  auto end = population.begin() + finish;
  Gaussian gaussian;
  for (; it < end; ++it){
    it->Mutate(mutation_perturbation_, mutation_rate_);
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

