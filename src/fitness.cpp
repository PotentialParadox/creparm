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
  if (am1_energies.size() <= 1)
    throw "Cannot find energy differences";
  auto am1_differences = dmath::Differences<double>(am1_energies.begin(), am1_energies.end());
  // Get HLT Energies
  std::vector<double> hlt_energies;
  for (auto &i: high_level_outputs_)
    hlt_energies.push_back(i.GetEnergy());
  if (am1_energies.size() != hlt_energies.size())
    throw "Energy vectors are not the same size";
  /* Convert to Differences */
  if (hlt_energies.size() <= 1)
    throw "Cannot find energy differences";
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

double reparm::Fitness::ExcitedFreqAverageFitness
(const reparm::ParameterGroup &param_group) const{
  /* Get the data */
  std::vector<std::vector<double> > am1_es_freqs;
  for (auto &i: param_group.GetOutputs())
    am1_es_freqs.push_back(i.GetESFrequencies());
  std::vector<std::vector<double> > hlt_es_freqs;
  for (auto &i: high_level_outputs_)
    hlt_es_freqs.push_back(i.GetESFrequencies());

  /* Convert to vector of averages */
  std::vector<double> am1_es_averages;
  for (auto &i: am1_es_freqs){
    double average = 0;
    average = dmath::Average(i.begin(), i.end());
    am1_es_averages.push_back(average);
  }
  std::vector<double> hlt_es_averages;
  for (auto &i: hlt_es_freqs){
    double average = 0;
    average = dmath::Average(i.begin(), i.end());
    hlt_es_averages.push_back(average);
  }

  /* Return the differences between the averages */
  if (am1_es_averages.size() != hlt_es_averages.size())
    throw "excited state sizes differ";
  return dmath::Distance(am1_es_averages.begin(),
			 am1_es_averages.end(),
			 hlt_es_averages.begin());
}

double reparm::Fitness::ExcitedFreqDiffFitness
(const reparm::ParameterGroup &param_group) const{
  /* Get the data */
  std::vector<std::vector<double> > am1_es_freqs;
  for (auto &i: param_group.GetOutputs())
    am1_es_freqs.push_back(i.GetESFrequencies());
  std::vector<std::vector<double> > hlt_es_freqs;
  for (auto &i: high_level_outputs_)
    hlt_es_freqs.push_back(i.GetESFrequencies());

  /* Convert to vector of differences */
  std::vector<std::vector<double> > am1_differences;
  for (auto &i: am1_es_freqs){
    if (i.size() <= 1)
      throw "Excited State Freq vector does not have enough elements";
    std::vector<double> differences;
    differences = dmath::Differences<double>(i.begin(), i.end());
    am1_differences.push_back(differences);
  }
  std::vector<std::vector<double> > hlt_differences;
  for (auto &i: hlt_es_freqs){
    if (i.size() <= 1)
      throw "Excited State Freq vector does not have enough elements";
    std::vector<double> differences;
    differences = dmath::Differences<double>(i.begin(), i.end());
    hlt_differences.push_back(differences);
  }

  /* Create a vector of distances */
  if (am1_differences.size() != hlt_differences.size())
    throw "Excited State sizes differ";
  std::vector<double> distances(am1_differences.size());
  auto am1_it = am1_differences.begin();
  auto end = am1_differences.end();
  auto hlt_it = hlt_differences.begin();
  for (; am1_it != end; ++am1_it, ++hlt_it){
    double distance = dmath::Distance(am1_it->begin(), am1_it->end(), hlt_it->begin());
    distances.push_back(distance);
  }

  return dmath::Average(distances.begin(), distances.end());
}

double reparm::Fitness::ExcitedIntAverageFitness
(const reparm::ParameterGroup &param_group) const{
  /* Get the data */
  std::vector<std::vector<double> > am1_es_intensities;
  for (auto &i: param_group.GetOutputs())
    am1_es_intensities.push_back(i.GetESIntensities());
  std::vector<std::vector<double> > hlt_es_intensities;
  for (auto &i: high_level_outputs_)
    hlt_es_intensities.push_back(i.GetESIntensities());
  if (am1_es_intensities.empty() || hlt_es_intensities.empty())
    throw "Cannot not obtain es_intensities";

  /* Make the vectors of averages */
  std::vector<double> am1_averages;
  for (auto &i: am1_es_intensities)
    am1_averages.emplace_back(dmath::Average(i.begin(), i.end()));
  std::vector<double> hlt_averages;
  for (auto &i: hlt_es_intensities)
    hlt_averages.emplace_back(dmath::Average(i.begin(), i.end()));

  if (am1_averages.size() != hlt_averages.size())
    throw "am1 and hlt excited intensities sizes differ";
  return dmath::Distance(am1_averages.begin(), am1_averages.end(), hlt_averages.begin());
}

reparm::Fitness::Fitness(const std::vector<reparm::ParameterGroup> &population,
                         const std::vector<reparm::GaussianOutput> &high_level_outputs)
  : high_level_outputs_{high_level_outputs}
  , energy_sigma_{0.0}
  , dipole_average_sigma_{0.0}
  , dipole_difference_sigma_(0.0)
  , excited_freq_avg_sigma_(0.0)
  , excited_freq_diff_sigma_(0.0)
  , excited_int_diff_sigma_(0.0)
    {
      /* Energy Values */
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

      /* Dipole Average Values */
      std::vector<double> dipole_avg_vals;
      for (const auto &i: population)
	try{
	  auto dipole = DipoleAverageFitness(i);
	  dipole_avg_vals.push_back(dipole);
	}catch(...){}
      if (dipole_avg_vals.size() <= 1)
	throw "Not enough dipoles. Try lowering the mutation rate.";
      dipole_average_sigma_ = dmath::STDEV(dipole_avg_vals.begin(), dipole_avg_vals.end());

      /* Dipole Differences Values */
      std::vector<double> dipole_diff_vals;
      for (const auto &i: population)
	try{
	  auto dipole = DipoleDifferenceFitness(i);
	  dipole_diff_vals.push_back(dipole);
	}catch(...){}
      if (dipole_diff_vals.size() <= 1)
	throw "Not enough dipoles. Try lowering the mutation rate.";
      dipole_difference_sigma_ = dmath::STDEV(dipole_diff_vals.begin(), dipole_diff_vals.end());

      /* Excited State Frequency Average Values */
      std::vector<double> es_freq_avg_vals;
      for (const auto &i: population)
	try{
	  auto es = ExcitedFreqAverageFitness(i);
	  es_freq_avg_vals.push_back(es);
	}catch(...){}
      if (es_freq_avg_vals.size() <= 1)
	throw "Not enough excited states. Try lowering the mutation rate.";
      excited_freq_avg_sigma_ = dmath::STDEV(es_freq_avg_vals.begin(), es_freq_avg_vals.end());

      /* Excited State Frequency Difference Values */
      std::vector<double> es_freq_diff_vals;
      for (const auto &i: population)
	try{
	  auto es = ExcitedFreqDiffFitness(i);
	  es_freq_diff_vals.push_back(es);
	}catch(...){}
      if (es_freq_diff_vals.size() <= 1)
	throw "Not enough excited states. Try lowering the mutation rate.";
      excited_freq_diff_sigma_ = dmath::STDEV(es_freq_diff_vals.begin(), es_freq_diff_vals.end());

      /* Excited State Intensities Average Values */
      std::vector<double> es_int_avg_vals;
      for (const auto &i: population)
	try{
	  auto es = ExcitedIntAverageFitness(i);
	  es_int_avg_vals.push_back(es);
	}catch(...){}
      if (es_int_avg_vals.size() <= 1)
	throw "Not enough excited state intensities.";
      excited_int_diff_sigma_ = dmath::STDEV(es_int_avg_vals.begin(), es_int_avg_vals.end());
      
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
    ss << "Dipole Difference Fitness: ";
    ss << dipole_difference_fitness / dipole_difference_sigma_ << std::endl;

    double excited_freq_average = ExcitedFreqAverageFitness(param_group);
    ss << "Excited State Frequency Average Fitness: ";
    ss << excited_freq_average / excited_freq_avg_sigma_ << std::endl;

    double excited_freq_difference = ExcitedFreqDiffFitness(param_group);
    ss << "Excited State Frequency Differences Fitness: ";
    ss << excited_freq_difference / excited_freq_diff_sigma_ << std::endl;

    double excited_int_average = ExcitedIntAverageFitness(param_group);
    ss << "Excited State Intensities Average Fitness: ";
    ss << excited_int_average << std::endl;
    ss << excited_int_diff_sigma_ << std::endl;

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
  double dd_fitness = 0;
  double efa_fitness = 0;  // Excited State Frequncy Average
  double efd_fitness = 0;  // Excited State Frequency Difference
  try{
    e_fitness = EnergyFitness(rhs);
    d_fitness = DipoleAverageFitness(rhs);
    dd_fitness = DipoleDifferenceFitness(rhs);
    efa_fitness = ExcitedFreqAverageFitness(rhs);
    efd_fitness = ExcitedFreqDiffFitness(rhs);
    fitness = (
	       e_fitness / energy_sigma_
               + d_fitness / dipole_average_sigma_
	       + dd_fitness / dipole_difference_sigma_
	       + efa_fitness / excited_freq_avg_sigma_
	       + efd_fitness / excited_freq_diff_sigma_
              );
  }
  catch(const char* e){
    std::cout << e << std::endl;
    fitness = 100000;
  }
  rhs.SetFitness(fitness);
  return fitness;
}

void reparm::Fitness::operator () (std::vector<reparm::ParameterGroup> &rhs) const{
  for (auto &i: rhs){
    double fitness = 0;
    double e_fitness = 0;
    double d_fitness = 0;
    double dd_fitness = 0;
    double efa_fitness = 0;  // Excited State Frequncy Average
    double efd_fitness = 0;  // Excited State Frequency Difference
    try{
      e_fitness = EnergyFitness(i);
      d_fitness = DipoleAverageFitness(i);
      dd_fitness = DipoleDifferenceFitness(i);
      efa_fitness = ExcitedFreqAverageFitness(i);
      efd_fitness = ExcitedFreqDiffFitness(i);
      fitness = (
		 e_fitness / energy_sigma_
		 + d_fitness / dipole_average_sigma_
		 + dd_fitness / dipole_difference_sigma_
		 + efa_fitness / excited_freq_avg_sigma_
		 + efd_fitness / excited_freq_diff_sigma_
		 );
    }
    catch(const char* e){
      std::cout << e << std::endl;
      fitness = 100000;
    }
    i.SetFitness(fitness);
  }
  sort(rhs.begin(), rhs.end());
}
