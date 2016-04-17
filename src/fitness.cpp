/* This is the source file you were looking for.
Well at least most likely. This file contains the implimentations
of the objective function of the genetic algorithm.
Created by Dustin Tracy (dtracy.uf@gmail.com) April 11, 2016 */
#include <fitness.h>
#include <vector>
#include <iostream>
#include <parameter_group.h>
#include <cmath>
#include <sstream>
#include "container_math.h"

double reparm::Fitness::EnergyFitness
(const reparm::ParameterGroup &param_group) const{
  // Get AM1 Energies
  std::vector<double> am1_energies;
  for (auto &i: param_group.GetOutputs())
    am1_energies.push_back(i.GetEnergy());
  /* Convert to Differences */ 
  auto am1_differences = dmath::Difference<double>(am1_energies.begin(), am1_energies.end());
  // Get HLT Energies
  std::vector<double> hlt_energies;
  for (auto &i: high_level_outputs_)
    hlt_energies.push_back(i.GetEnergy());
  if (am1_energies.size() != hlt_energies.size())
    throw "Energy vectors are not the same size";
  /* Convert to Differences */
  auto hlt_differences = dmath::Difference<double>(hlt_energies.begin(), hlt_energies.end());
  auto distance = dmath::Distance(am1_differences.begin(), am1_differences.end(),
				  hlt_differences.begin());
  return std::sqrt(distance/am1_energies.size());
}

double reparm::Fitness::DipoleAverageFitness
(const reparm::ParameterGroup &param_group)const{
  std::vector<std::vector<double> > am1_dipoles;
  for (auto &i: param_group.GetOutputs())
    am1_dipoles.push_back(i.GetDipole());
  std::vector<double> am1_average(3);
  for (auto &i: am1_dipoles){
    for (size_t j = 0; j != i.size(); ++j){
      am1_average[j] += i[j];
    }
  }
  for (auto &i: am1_average) i /= static_cast<double>(am1_dipoles.size());
  std::vector<std::vector<double> > hlt_dipoles;
  for (auto &i: high_level_outputs_)
    hlt_dipoles.push_back(i.GetDipole());
  std::vector<double> hlt_average(3);
  for (auto &i: hlt_dipoles){
    for (size_t j = 0; j != i.size(); ++j){
      hlt_average[j] += i[j];
    }
  }
  for (auto &i: hlt_average) i /= static_cast<double>(hlt_dipoles.size());
  return dmath::Distance(am1_average.begin(), am1_average.end(),
			 hlt_average.begin());
}

reparm::Fitness::Fitness(const std::vector<reparm::ParameterGroup> &population,
                         const std::vector<reparm::GaussianOutput> &high_level_outputs)
  : high_level_outputs_{high_level_outputs}
  , energy_sigma_{0.0}
  , dipole_average_sigma_{0.0}
    {
      std::vector<double> energy_values;
      for (const auto &i: population)
	energy_values.emplace_back(EnergyFitness(i));
      energy_sigma_ = dmath::STDEV(energy_values.begin(), energy_values.end());
      std::vector<double> dipole_avg_vals;
      for (const auto &i: population)
      	dipole_avg_vals.emplace_back(DipoleAverageFitness(i));
      dipole_average_sigma_ = dmath::STDEV(dipole_avg_vals.begin(), dipole_avg_vals.end());
    }

std::string reparm::Fitness::StringList(const reparm::ParameterGroup &param_group) const{
  std::stringstream ss;

  try{
    double energy_fitness = EnergyFitness(param_group);
    ss << "Energy Fitness: ";
    ss << energy_fitness / energy_sigma_ << std::endl;

    double dipole_average_fitness = DipoleAverageFitness(param_group);
    ss << "Dipole Fitness: ";
    ss << dipole_average_fitness / dipole_average_sigma_ << std::endl;

  }
  catch(const char* e){
    std::cout << "Ignoring Step" << std::endl;
  }

  return ss.str();
}

double reparm::Fitness::operator () (reparm::ParameterGroup &rhs) const{
  double fitness = 0;
  double e_fitness = 0;
  double d_fitness = 0;
  try{
    e_fitness = EnergyFitness(rhs);
    d_fitness = DipoleAverageFitness(rhs);
    fitness = (
	       e_fitness / energy_sigma_
               + d_fitness / dipole_average_sigma_
              );
  }
  catch(const char* e){
    std::cout << e << std::endl;
    fitness = 10;
  }
  rhs.SetFitness(fitness);
  return fitness;
}

void reparm::Fitness::operator () (std::vector<reparm::ParameterGroup> &rhs) const{
  for (auto &i: rhs){
    double fitness = 0;
    double e_fitness = 0;
    double d_fitness = 0;
    try{
      e_fitness = EnergyFitness(i);
      d_fitness = DipoleAverageFitness(i);
      fitness = (
		 e_fitness / energy_sigma_
		 + d_fitness / dipole_average_sigma_
		 );
    }
    catch(const char* e){
      std::cout << e << std::endl;
      fitness = 10;
    }
    i.SetFitness(fitness);
  }
  sort(rhs.begin(), rhs.end());
}
