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
#include <armadillo>
#include "container_math.h"

double reparm::Fitness::EnergyFitness
(const reparm::ParameterGroup &param_group) const{
  // Get AM1 Energies
  std::vector<double> am1_energies;
  for (auto &i: param_group.GetOutputs())
    am1_energies.push_back(i.GetEnergy());
  /* Convert to Differences */ 
  if (am1_energies.size() <= 1)
    throw "Cannot find am1 energy differences";
  auto am1_differences = dmath::DifferenceMatrix<double>(am1_energies);
  // Get HLT Energies
  std::vector<double> hlt_energies;
  for (auto &i: high_level_outputs_)
    hlt_energies.push_back(i.GetEnergy());
  if (am1_energies.size() != hlt_energies.size())
    throw "Energy vectors are not the same size";
  /* Convert to Differences */
  if (hlt_energies.size() <= 1)
    throw "Cannot find hlt energy differences";
  auto hlt_differences = dmath::DifferenceMatrix<double>(hlt_energies);
  auto distance = dmath::Distance(am1_differences.begin(), am1_differences.end(),
				  hlt_differences.begin());
  return distance;
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
  /* Get The Dipoles */
  std::vector<std::vector<double> > am1_dipoles;
  for (auto &i: param_group.GetOutputs())
    am1_dipoles.push_back(i.GetDipole());
  if (am1_dipoles.empty())
    throw "Dipoles not found";
  std::vector<std::vector<double> > hlt_dipoles;
  for (auto &i: high_level_outputs_)
    hlt_dipoles.push_back(i.GetDipole());
  if (hlt_dipoles.empty())
    throw "Dipoles not found";

  /* Convert into difference matrix */
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

double reparm::Fitness::ExcitedIntDiffFitness
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

  /* Create vectors of the differences */
  std::vector<std::vector<double> > am1_differences;
  for (auto &i: am1_es_intensities)
    am1_differences.emplace_back(dmath::Differences<double>(i.begin(), i.end()));
  std::vector<std::vector<double> > hlt_differences;
  for (auto &i: hlt_es_intensities)
    hlt_differences.emplace_back(dmath::Differences<double>(i.begin(), i.end()));
  if (hlt_differences.size() != am1_differences.size())
    throw "amount of excited states differ";

  /* Now make a vector of the distances between hlt and am1 */
  std::vector<double> distances;
  auto am1_it = am1_differences.begin();
  auto hlt_it = hlt_differences.begin();
  auto end = am1_differences.end();
  for (; am1_it != end; ++am1_it, ++hlt_it){
    if (am1_it->size() != hlt_it->size())
      throw "excited intensities differences differ";
    distances.emplace_back(dmath::Distance(am1_it->begin()
					   , am1_it->end(), hlt_it->begin()));
  }

  return dmath::Average(distances.begin(), distances.end());
}

double reparm::Fitness::IRFreqAverageFitness
(const reparm::ParameterGroup &param_group) const{
  /* Get the data */
  std::vector<std::vector<double> > am1_ir_freqs;
  for (auto &i: param_group.GetOutputs())
    am1_ir_freqs.push_back(i.GetFrequencies());
  std::vector<std::vector<double> > hlt_ir_freqs;
  for (auto &i: high_level_outputs_)
    hlt_ir_freqs.push_back(i.GetFrequencies());
  if (am1_ir_freqs.empty() || hlt_ir_freqs.empty())
    throw "IR frequencies not found";

  /* Convert to vector of averages */
  std::vector<double> am1_ir_averages;
  for (auto &i: am1_ir_freqs){
    double average = 0;
    average = dmath::Average(i.begin(), i.end());
    am1_ir_averages.push_back(average);
  }
  std::vector<double> hlt_ir_averages;
  for (auto &i: hlt_ir_freqs){
    double average = 0;
    average = dmath::Average(i.begin(), i.end());
    hlt_ir_averages.push_back(average);
  }

  /* Return the differences between the averages */
  if (am1_ir_averages.size() != hlt_ir_averages.size())
    throw "infrared sizes differ";
  return dmath::Distance(am1_ir_averages.begin(),
			 am1_ir_averages.end(),
			 hlt_ir_averages.begin());
}

double reparm::Fitness::IRFreqDiffFitness
(const reparm::ParameterGroup &param_group) const{
  /* Get the data */
  std::vector<std::vector<double> > am1_ir_freqs;
  for (auto &i: param_group.GetOutputs())
    am1_ir_freqs.push_back(i.GetFrequencies());
  std::vector<std::vector<double> > hlt_ir_freqs;
  for (auto &i: high_level_outputs_)
    hlt_ir_freqs.push_back(i.GetFrequencies());

  /* Convert to vector of differences */
  std::vector<std::vector<double> > am1_differences;
  for (auto &i: am1_ir_freqs){
    if (i.size() <= 1)
      throw "IR Freq vector does not have enough elements";
    std::vector<double> differences;
    differences = dmath::Differences<double>(i.begin(), i.end());
    am1_differences.push_back(differences);
  }
  std::vector<std::vector<double> > hlt_differences;
  for (auto &i: hlt_ir_freqs){
    if (i.size() <= 1)
      throw "IR Freq vector does not have enough elements";
    std::vector<double> differences;
    differences = dmath::Differences<double>(i.begin(), i.end());
    hlt_differences.push_back(differences);
  }

  /* Create a vector of distances */
  if (am1_differences.size() != hlt_differences.size())
    throw "IR Frequency sizes differ";
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

double reparm::Fitness::IRIntensityAverageFitness
(const reparm::ParameterGroup &param_group) const{
  /* Get the data */
  std::vector<std::vector<double> > am1_ir_intensities;
  for (auto &i: param_group.GetOutputs())
    am1_ir_intensities.push_back(i.GetIntensities());
  std::vector<std::vector<double> > hlt_ir_intensities;
  for (auto &i: high_level_outputs_)
    hlt_ir_intensities.push_back(i.GetIntensities());
  if (am1_ir_intensities.empty() || hlt_ir_intensities.empty())
    throw "Cannot not obtain ir_intensities";

  /* Make the vectors of averages */
  std::vector<double> am1_averages;
  for (auto &i: am1_ir_intensities)
    am1_averages.emplace_back(dmath::Average(i.begin(), i.end()));
  std::vector<double> hlt_averages;
  for (auto &i: hlt_ir_intensities)
    hlt_averages.emplace_back(dmath::Average(i.begin(), i.end()));

  if (am1_averages.size() != hlt_averages.size())
    throw "am1 and hlt ir intensities sizes differ";
  return dmath::Distance(am1_averages.begin(), am1_averages.end(), hlt_averages.begin());
}

double reparm::Fitness::IRIntensityDiffFitness
(const reparm::ParameterGroup &param_group) const{
  /* Get the data */
  std::vector<std::vector<double> > am1_ir_intensities;
  for (auto &i: param_group.GetOutputs())
    am1_ir_intensities.push_back(i.GetIntensities());
  std::vector<std::vector<double> > hlt_ir_intensities;
  for (auto &i: high_level_outputs_)
    hlt_ir_intensities.push_back(i.GetIntensities());
  if (am1_ir_intensities.empty() || hlt_ir_intensities.empty())
    throw "Cannot not obtain ir_intensities";

  /* Create vectors of the differences */
  std::vector<std::vector<double> > am1_differences;
  for (auto &i: am1_ir_intensities)
    am1_differences.emplace_back(dmath::Differences<double>(i.begin(), i.end()));
  std::vector<std::vector<double> > hlt_differences;
  for (auto &i: hlt_ir_intensities)
    hlt_differences.emplace_back(dmath::Differences<double>(i.begin(), i.end()));
  if (hlt_differences.size() != am1_differences.size())
    throw "amount of normal modes differ";

  /* Now make a vector of the distances between hlt and am1 */
  std::vector<double> distances;
  auto am1_it = am1_differences.begin();
  auto hlt_it = hlt_differences.begin();
  auto end = am1_differences.end();
  for (; am1_it != end; ++am1_it, ++hlt_it){
    if (am1_it->size() != hlt_it->size())
      throw "ir intensities differences differ";
    distances.emplace_back(dmath::Distance(am1_it->begin()
					   , am1_it->end(), hlt_it->begin()));
  }

  return dmath::Average(distances.begin(), distances.end());
}

double reparm::Fitness::ForceGeomDiffFitness
(const reparm::ParameterGroup &param_group) const{
  /* Get the data */
  std::vector<std::vector<double> > am1_raw_forces;
  for (auto &i: param_group.GetOutputs())
    am1_raw_forces.push_back(i.GetForces());
  std::vector<std::vector<double> > hlt_raw_forces;
  for (auto &i: high_level_outputs_)
    hlt_raw_forces.push_back(i.GetForces());
  if (am1_raw_forces.size() < 2)
    throw "not enough geometries";

  /* Currently forces are stored as a one 3n long
     array, where n is the number of force vectors.
     I'm going to convert this into a n long vector
     of vectors of size 3.
  */
  typedef std::vector<double> force;

  std::vector<std::vector<force> > am1_forces;
  /* for each geometry */
  for (auto &i: am1_raw_forces){
    std::vector<force> forces;
    forces = dmath::VectorOneToTwo(i, 3);
    am1_forces.push_back(forces);
  }
  std::vector<std::vector<force> > hlt_forces;
  /* for each geometry */
  for (auto &i: hlt_raw_forces){
    std::vector<force> forces;
    forces = dmath::VectorOneToTwo(i, 3);
    hlt_forces.push_back(forces);
  }

  /* We now need to find the vector differences
     between the geometries 
  */
  std::vector<std::vector<force> > am1_differences;
  auto geom1 = am1_forces.begin();
  auto geom2 = geom1 + 1;
  auto geom_end = am1_forces.end();
  for (; geom2 != geom_end; ++geom1, ++geom2){
    std::vector<force> force_differences;
    auto force1 = geom1->begin();
    auto force2 = geom2->begin();
    auto force_end = geom1->end();
    for (; force1 != force_end; ++force1, ++force2){
      force difference;
      difference = dmath::VectorDifference<double>(force1->begin(), force1->end(), force2->begin());
      force_differences.push_back(difference);
    }
    am1_differences.push_back(force_differences);
  }
  std::vector<std::vector<force> > hlt_differences;
  geom1 = hlt_forces.begin();
  geom2 = geom1 + 1;
  geom_end = hlt_forces.end();
  for (; geom2 != geom_end; ++geom1, ++geom2){
    std::vector<force> force_differences;
    auto force1 = geom1->begin();
    auto force2 = geom2->begin();
    auto force_end = geom1->end();
    for (; force1 != force_end; ++force1, ++force2){
      force difference;
      difference = dmath::VectorDifference<double>(force1->begin(), force1->end(), force2->begin());
      force_differences.push_back(difference);
    }
    hlt_differences.push_back(force_differences);
  }

  /* The differences vectors now hold the number of geometries - 1 values
     representing the vector difference between the force in a geometry to that
     of the geometry immediately following it. We now find the average distance
     between the value of the two theories.
  */
  double distance = 0;
  auto am1_geom = am1_differences.begin();
  auto hlt_geom = hlt_differences.begin();
  auto am1_geom_end = am1_differences.end();
  for (; am1_geom != am1_geom_end; ++am1_geom, ++hlt_geom){
    auto am1_force = am1_geom->begin();
    auto hlt_force = hlt_geom->begin();
    auto am1_force_end = am1_geom->end();
    double distance_per_geometry = 0;
    for (; am1_force != am1_force_end; ++am1_force, ++hlt_force){
      double distance_per_force = 0;
      distance_per_force =
	dmath::Distance(am1_force->begin(), am1_force->end(), hlt_force->end());
      distance_per_geometry += distance_per_force;
    }
    distance_per_geometry /= am1_geom->size();
    distance += distance_per_geometry;
  }

  return distance /= am1_differences.size();
}

reparm::Fitness::Fitness(const std::vector<reparm::ParameterGroup> &population,
                         const std::vector<reparm::GaussianOutput> &high_level_outputs)
  : high_level_outputs_{high_level_outputs}
  , energy_sigma_{0.0}
  , dipole_average_sigma_{0.0}
  , dipole_difference_sigma_(0.0)
  , excited_freq_avg_sigma_(0.0)
  , excited_freq_diff_sigma_(0.0)
  , excited_int_avg_sigma_(0.0)
  , excited_int_diff_sigma_(0.0)
  , ir_freq_avg_sigma_(0.0)
  , ir_freq_diff_sigma_(0.0)
  , ir_int_avg_sigma_(0.0)
  , ir_int_diff_sigma_(0.0)
  , force_geom_diff_sigma_(0.0)
  , force_geom_avg_sigma_(0.0)
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
      excited_int_avg_sigma_ = dmath::STDEV(es_int_avg_vals.begin(), es_int_avg_vals.end());

      /* Excited State Intensities Difference Values */
      std::vector<double> es_int_diff_vals;
      for (const auto &i: population)
	try{
	  auto es = ExcitedIntDiffFitness(i);
	  es_int_diff_vals.push_back(es);
	}catch(...){}
      if (es_int_diff_vals.size() <= 1)
	throw "Not enough excited state intensities.";
      excited_int_diff_sigma_ = dmath::STDEV(es_int_diff_vals.begin(), es_int_diff_vals.end());
      
      /* IR Frequency Average Values */
      std::vector<double> ir_freq_avg_vals;
      for (const auto &i: population)
	try{
	  auto ir = IRFreqAverageFitness(i);
	  ir_freq_avg_vals.push_back(ir);
	}catch(...){}
      if (ir_freq_avg_vals.size() <= 1)
	throw "Not enough normal modes. Try lowering the mutation rate.";
      ir_freq_avg_sigma_ = dmath::STDEV(ir_freq_avg_vals.begin(), ir_freq_avg_vals.end());

      /* IR Frequency Difference Values */
      std::vector<double> ir_freq_diff_vals;
      for (const auto &i: population)
	try{
	  auto ir = IRFreqDiffFitness(i);
	  ir_freq_diff_vals.push_back(ir);
	}catch(...){}
      if (ir_freq_diff_vals.size() <= 1)
	throw "Not enough normal modes. Try lowering the mutation rate.";
      ir_freq_diff_sigma_ = dmath::STDEV(ir_freq_diff_vals.begin(), ir_freq_diff_vals.end());

      /* IR Intensities Average Values */
      std::vector<double> ir_int_avg_vals;
      for (const auto &i: population)
	try{
	  auto ir = IRIntensityAverageFitness(i);
	  ir_int_avg_vals.push_back(ir);
	}catch(...){}
      if (ir_int_avg_vals.size() <= 1)
	throw "Not enough normal modes. Try lowering the mutation rate.";
      ir_int_avg_sigma_ = dmath::STDEV(ir_int_avg_vals.begin(), ir_int_avg_vals.end());

      /* IR Intensities Difference Values */
      std::vector<double> ir_int_diff_vals;
      for (const auto &i: population)
	try{
	  auto ir = IRIntensityDiffFitness(i);
	  ir_int_diff_vals.push_back(ir);
	}catch(...){}
      if (ir_int_diff_vals.size() <= 1)
	throw "Not enough normal modes. Try lowering the mutation rate.";
      ir_int_diff_sigma_ = dmath::STDEV(ir_int_diff_vals.begin(), ir_int_diff_vals.end());

      /* Force Geometry Difference Values */
      std::vector<double> force_geom_diff_vals;
      for (const auto &i: population)
	try{
	  auto force = ForceGeomDiffFitness(i);
	  force_geom_diff_vals.push_back(force);
	}catch(...){}
      if (force_geom_diff_vals.size() <= 1)
	throw "Not enough forces. Try lowering the mutation rate.";
      force_geom_diff_sigma_ = dmath::STDEV(force_geom_diff_vals.begin(), force_geom_diff_vals.end());

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
    ss << excited_int_average / excited_int_avg_sigma_ << std::endl;

    double excited_int_differences = ExcitedIntDiffFitness(param_group);
    ss << "Excited State Intensities Difference Fitness: ";
    ss << excited_int_differences / excited_int_diff_sigma_ << std::endl;

    double ir_freq_average = IRFreqAverageFitness(param_group);
    ss << "IR Frequency Average Fitness: ";
    ss << ir_freq_average / ir_freq_avg_sigma_ << std::endl;

    double ir_freq_difference = IRFreqDiffFitness(param_group);
    ss << "IR Frequency Differences Fitness: ";
    ss << ir_freq_difference / ir_freq_diff_sigma_ << std::endl;

    double ir_int_average = IRIntensityAverageFitness(param_group);
    ss << "IR Intensities Average Fitness: ";
    ss << ir_int_average / ir_int_avg_sigma_ << std::endl;

    double ir_int_differences = IRIntensityDiffFitness(param_group);
    ss << "IR Intensities Difference Fitness: ";
    ss << ir_int_differences / ir_int_diff_sigma_ << std::endl;

    double force_geom_differences = ForceGeomDiffFitness(param_group);
    ss << "Force Geometry Difference Fitness: ";
    ss << force_geom_differences / force_geom_diff_sigma_ << std::endl;

  }
  catch(const char* e){
    std::cout << "Ignoring Step because" << std::endl;
    std::cout << e << std::endl;
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
  double eia_fitness = 0; // Excited State Intensity Average
  double eid_fitness = 0;  // Excited State Intensity Difference
  double irfa_fitness = 0;  // IR Frequncy Average
  double irfd_fitness = 0;  // IR Frequency Difference
  double iria_fitness = 0; // IR Intensity Average
  double irid_fitness = 0;  // IR Intensity Difference
  try{
    e_fitness = EnergyFitness(rhs);
    d_fitness = DipoleAverageFitness(rhs);
    dd_fitness = DipoleDifferenceFitness(rhs);
    efa_fitness = ExcitedFreqAverageFitness(rhs);
    efd_fitness = ExcitedFreqDiffFitness(rhs);
    eia_fitness = ExcitedIntAverageFitness(rhs);
    eid_fitness = ExcitedIntDiffFitness(rhs);
    irfa_fitness = IRFreqAverageFitness(rhs);
    irfd_fitness = IRFreqDiffFitness(rhs);
    iria_fitness = IRIntensityAverageFitness(rhs);
    irid_fitness = IRIntensityDiffFitness(rhs);
    fitness = (
	       e_fitness / energy_sigma_
               + d_fitness / dipole_average_sigma_
	       + dd_fitness / dipole_difference_sigma_
	       + efa_fitness / excited_freq_avg_sigma_
	       + efd_fitness / excited_freq_diff_sigma_
	       + eia_fitness / excited_int_avg_sigma_
	       + eid_fitness / excited_int_diff_sigma_
	       + irfa_fitness / ir_freq_avg_sigma_
	       + irfd_fitness / ir_freq_diff_sigma_
	       + iria_fitness / ir_int_avg_sigma_
	       + irid_fitness / ir_int_diff_sigma_
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
    double eia_fitness = 0; // Excited State Intensity Average
    double eid_fitness = 0;  // Excited State Intensity Difference
    double irfa_fitness = 0;  // IR Frequncy Average
    double irfd_fitness = 0;  // IR Frequency Difference
    double iria_fitness = 0; // IR Intensity Average
    double irid_fitness = 0;  // IR Intensity Difference
    try{
      e_fitness = EnergyFitness(i);
      d_fitness = DipoleAverageFitness(i);
      dd_fitness = DipoleDifferenceFitness(i);
      efa_fitness = ExcitedFreqAverageFitness(i);
      efd_fitness = ExcitedFreqDiffFitness(i);
      eia_fitness = ExcitedIntAverageFitness(i);
      eid_fitness = ExcitedIntDiffFitness(i);
      irfa_fitness = IRFreqAverageFitness(i);
      irfd_fitness = IRFreqDiffFitness(i);
      iria_fitness = IRIntensityAverageFitness(i);
      irid_fitness = IRIntensityDiffFitness(i);
      fitness = (
		 e_fitness / energy_sigma_
		 + d_fitness / dipole_average_sigma_
		 + dd_fitness / dipole_difference_sigma_
		 + efa_fitness / excited_freq_avg_sigma_
		 + efd_fitness / excited_freq_diff_sigma_
		 + eia_fitness / excited_int_avg_sigma_
		 + eid_fitness / excited_int_diff_sigma_
		 + irfa_fitness / ir_freq_avg_sigma_
		 + irfd_fitness / ir_freq_diff_sigma_
		 + iria_fitness / ir_int_avg_sigma_
		 + irid_fitness / ir_int_diff_sigma_
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
