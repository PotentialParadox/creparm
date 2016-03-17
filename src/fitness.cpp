#include <fitness.h>
#include <vector>
#include <iostream>
#include <parameter_group.h>
#include <cmath>
#include <sstream>

double Average(std::vector<double> &a){
  double value = 0;
  for (auto &i: a){ value += i;}
  return value / static_cast<double>(a.size());
}

double RMS(const std::vector<double> &a,const std::vector<double> &b){
  if (a.size() != b.size())
    throw "RMS Failure: vector sizes are not the same";
  double variance = 0;
  for (size_t i = 0; i != a.size(); ++i)
    variance += pow(a[i] - b[i], 2);
  variance /= static_cast<double>(a.size());
  return sqrt(variance);
}

double AverageRMS(const std::vector<std::vector<double> > &a,
                  const std::vector<std::vector<double> > &b){
  if (a.size() != b.size())
    throw "RMS Failure: vector sizes are not the same";
  double avearge_rms = 0;
  for (size_t i = 0; i < a.size(); ++i)
    avearge_rms += RMS(a[i], b[i]);
  return avearge_rms / static_cast<double>(a.size());
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

double DipoleFitness(const reparm::ParameterGroup &param_group,
                     const std::vector<reparm::GaussianOutput> &high_level_outputs){
  // Get AM1 Dipoles
  std::vector<std::vector<double> > am1_dipoles;
  for (const auto &i: param_group.GetOutputs())
    am1_dipoles.push_back(i.GetDipole());
  // Get HLT Dipoles
  std::vector<std::vector<double> > hlt_dipoles;
  for (const auto &i: high_level_outputs)
    hlt_dipoles.push_back(i.GetDipole());
  return AverageRMS(am1_dipoles, hlt_dipoles);
}

double ExcitedStateFitness(const reparm::ParameterGroup &param_group,
                           const std::vector<reparm::GaussianOutput> &high_level_outputs){
  // Get AM1 Excited States
  std::vector<std::vector<double> > am1_excited_states;
  for (const auto &i: param_group.GetOutputs())
    am1_excited_states.push_back(i.GetExcitedStates());
  // Get HLT Excited States
  std::vector<std::vector<double> > hlt_excited_states;
  for (const auto &i: high_level_outputs)
    hlt_excited_states.push_back(i.GetExcitedStates());
  return AverageRMS(am1_excited_states, hlt_excited_states);
}

reparm::Fitness::Fitness(const reparm::ParameterGroup &param_group,
                         const std::vector<reparm::GaussianOutput> &high_level_outputs)
  : high_level_outputs_{high_level_outputs}
{
  original_e_fitness_ = EnergyFitness(param_group, high_level_outputs_);
  original_d_fitness_ = DipoleFitness(param_group, high_level_outputs_);
  original_es_fitness_ = ExcitedStateFitness(param_group, high_level_outputs_);
} 

std::string reparm::Fitness::StringList(const reparm::ParameterGroup &param_group) const{
  std::stringstream ss;

  try{
    double e_fitness = (EnergyFitness(param_group, high_level_outputs_) / original_e_fitness_);
    ss << "Energy Fitness: ";
    ss << e_fitness << std::endl;

    double d_fitness = (DipoleFitness(param_group, high_level_outputs_) / original_d_fitness_);
    ss << "Dipole Fitness: ";
    ss << d_fitness << std::endl;

    double es_fitness = (ExcitedStateFitness(param_group, high_level_outputs_) / 
                         original_es_fitness_);
    ss << "Excited State Fitness: ";
    ss << es_fitness << std::endl;
  }
  catch(const char* e){
    std::cout << "Ignoring Step" << std::endl;
  }

  return ss.str();
}

double reparm::Fitness::operator () (reparm::ParameterGroup &rhs) const{
  double fitness;
  double e_fitness = 0;
  double d_fitness = 0;
  double es_fitness = 0;
  try{
    e_fitness = (EnergyFitness(rhs, high_level_outputs_) / original_e_fitness_);
    d_fitness = (DipoleFitness(rhs, high_level_outputs_) / original_d_fitness_);
    es_fitness = (ExcitedStateFitness(rhs, high_level_outputs_) / original_es_fitness_);
    double fit_sum = e_fitness + d_fitness + es_fitness;
    fitness = ( (e_fitness / fit_sum) * e_fitness +
                       (d_fitness / fit_sum) * d_fitness +
                       (es_fitness / fit_sum) * es_fitness );
  }
  catch(const char* e){
    std::cout << e << std::endl;
    fitness = 10;
  }
  rhs.SetFitness(fitness);
  return fitness;
}
