/* This is the source file you were looking for.
Well at least most likely. This file contains the implimentations
of the objective function of the genetic algorithm.
Created by Dustin Tracy (dtracy.uf@gmail.com) April 11, 2016 */
#include <fitness.h>
#include <algorithm>
#include <numeric>
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
  auto am1_differences = dmath::Differences<double>(am1_energies.begin(), am1_energies.end());
  // Get HLT Energies
  std::vector<double> hlt_energies;
  for (auto &i: high_level_outputs_)
    hlt_energies.push_back(i.GetEnergy());
  if (am1_energies.size() != hlt_energies.size())
    throw "Energy vectors are not the same size";
  /* Convert to Differences */
  auto hlt_differences = dmath::Differences<double>(hlt_energies.begin(), hlt_energies.end());
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
  if (am1_dipoles.size() != hlt_dipoles.size())
    throw "Dipole vectors are not equal";
  for (auto &i: hlt_average) i /= static_cast<double>(hlt_dipoles.size());
  return dmath::Distance(am1_average.begin(), am1_average.end(),
			 hlt_average.begin());
}

double reparm::Fitness::DipoleDifferenceFitness
(const reparm::ParameterGroup &param_group) const{
  /* Get AM1 Dipole */
  std::vector<std::vector<double> > am1_dipoles;
  for (auto &i: param_group.GetOutputs())
    am1_dipoles.push_back(i.GetDipole());
  if (am1_dipoles.empty())
    throw "Dipoles not found";
  std::vector<std::vector<double> > am1_differences;
  auto it = am1_dipoles.begin();
  auto it1 = next(it, 1);
  auto end = am1_dipoles.end();
  for (; it1 != end; ++it, ++it1){
    am1_differences.emplace_back(dmath::VectorDifference<double>(
								 it->begin(),
								 it->end(),
								 it1->begin()
								 )
				 );
  }
  /* Get HLT Dipole */
  std::vector<std::vector<double> > hlt_dipoles;
  for (auto &i: high_level_outputs_)
    hlt_dipoles.push_back(i.GetDipole());
  if (hlt_dipoles.empty())
    throw "Dipoles not found";
  std::vector<std::vector<double> > hlt_differences;
  it = hlt_dipoles.begin();
  it1 = next(it, 1);
  end = hlt_dipoles.end();
  for (; it1 != end; ++it, ++it1){
    hlt_differences.emplace_back(dmath::VectorDifference<double>(
								 it->begin(),
								 it->end(),
								 it1->begin()
								 )
				 );
  }

  /* Now Calculate the average distance between the distance vectors in
     the am1 and hlt calculations */
  if (am1_differences.size() != hlt_differences.size())
    throw "Dipole differences sizes differ";
  std::vector<double> distances;
  it = am1_differences.begin();
  it1 = hlt_differences.begin();
  end = am1_differences.end();
  for(; it != end; ++it, ++it1){
    distances.emplace_back(dmath::Distance(it->begin(), it->end(), it1->begin()));
  }
  auto average = dmath::Average(distances.begin(), distances.end());
  return average;
}

reparm::Fitness::Fitness(const std::vector<reparm::ParameterGroup> &population,
                         const std::vector<reparm::GaussianOutput> &high_level_outputs)
  : high_level_outputs_{high_level_outputs}
  , energy_sigma_{0.0}
  , dipole_average_sigma_{0.0}
    {
      std::vector<double> energy_values;
      for (const auto &i: population){
	try{
	  auto energy = EnergyFitness(i);
	  energy_values.push_back(energy);
	}catch(...){}
      }
      if (energy_values.size() <= 1)
	throw "Not enough energies. Try lowering the mutation rate.";
      energy_sigma_ = dmath::STDEV(energy_values.begin(), energy_values.end());
      std::vector<double> dipole_avg_vals;
      for (const auto &i: population)
	try{
	  auto dipole = DipoleAverageFitness(i);
	  dipole_avg_vals.push_back(dipole);
	}catch(...){}
      if (dipole_avg_vals.size() <= 1)
	throw "Not enough dipoles. Try lowering the mutation rate.";
      dipole_average_sigma_ = dmath::STDEV(dipole_avg_vals.begin(), dipole_avg_vals.end());
    }

std::string reparm::Fitness::StringList(const reparm::ParameterGroup &param_group) const{
  std::stringstream ss;

  try{
    double energy_fitness = EnergyFitness(param_group);
    ss << "Energy Fitness: ";
    ss << energy_fitness / energy_sigma_ << std::endl;

    double dipole_average_fitness = DipoleAverageFitness(param_group);
    ss << "Dipole Average Fitness: ";
    ss << dipole_average_fitness / dipole_average_sigma_ << std::endl;

    double dipole_difference_fitness = DipoleDifferenceFitness(param_group);
    ss << "Dipole Differnce Fitness: ";
    ss << dipole_difference_fitness << std::endl;

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
