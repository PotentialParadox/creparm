#include <fitness.h>
#include <vector>
#include <iostream>
#include <parameter_group.h>
#include <cmath>

double RMS(std::vector<double> &a, std::vector<double> &b){
  if (a.size() != b.size())
    throw "RMS Failure";
  double variance = 0;
  for (size_t i = 0; i != a.size(); ++i)
    variance += pow(a[i] - b[i], 2);
  variance /= static_cast<double>(a.size());
  return sqrt(variance);
}

double Average(std::vector<double> &a){
  double value = 0;
  for (auto &i: a){ value += i;}
  return value / static_cast<double>(a.size());
}

double EnergyFitness(const reparm::ParameterGroup &param_group,
                     const std::vector<reparm::GaussianOutput> &high_level_outputs){
  // Get AM1 Energies
  std::vector<double> am1_energies;
  for (auto &i: param_group.GetOutputs())
    am1_energies.push_back(i.GetEnergy());
  // Get HLT Energies
  std::vector<double> hlt_energies;
  for (auto &i: high_level_outputs)
    hlt_energies.push_back(i.GetEnergy());
  // Find the difference between the units
  double difference = Average(am1_energies) - Average(hlt_energies);
  // Convert the AM1 energies to the HLT units
  for (auto &i: am1_energies)
    i -= difference;
  // Return the RMS
  return RMS(am1_energies, hlt_energies);
}

reparm::Fitness::Fitness(const reparm::ParameterGroup &param_group,
                         const std::vector<reparm::GaussianOutput> &high_level_outputs){
  high_level_outputs_ = high_level_outputs;
  original_fitness_.push_back(EnergyFitness(param_group, high_level_outputs_));
} 

double reparm::Fitness::operator () (reparm::ParameterGroup &rhs){
  double fitness = 0;
  try{
    fitness = (EnergyFitness(rhs, high_level_outputs_) / original_fitness_[0]);
  }
  catch(const char* e){
    std::cout << e << std::endl;
    return 10;
  }
  rhs.SetFitness(fitness);
  return fitness;
}
