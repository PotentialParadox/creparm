/* This functor is in charge of calculating the cost function in the genetic algorithm.
 * This is most likely the class that you will want to modify.
 * Note that this updates the fitness members found in the ParameterGroup class
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 3/8/2016 */
#ifndef FITNESS_H
#define FITNESS_H

#include <vector>
#include <parameter_group.h>
#include <functional>

namespace reparm{

class Fitness{
  friend class Mutate;

  private:
    std::vector<reparm::GaussianOutput> high_level_outputs_;
    double energy_sigma_;  // Energy Fitness
    double dipole_average_sigma_;  // Dipole Average Fitness
    double dipole_difference_sigma_;  // Dipole Difference Fitness
    double excited_freq_avg_sigma_;  // Exctied State Frequencies average
    double excited_freq_diff_sigma_;  // Excited State Frequencies differences
    double excited_int_avg_sigma_;  // Exctied State Intensities average
    double excited_int_diff_sigma_;  // Excited State Intensities differences
    double EnergyFitness(const reparm::ParameterGroup &param_group) const;
    double DipoleAverageFitness(const reparm::ParameterGroup &param_group) const;
    double DipoleDifferenceFitness(const reparm::ParameterGroup &param_group) const;
    double ExcitedFreqAverageFitness(const reparm::ParameterGroup &param_group) const;
    double ExcitedFreqDiffFitness(const reparm::ParameterGroup &param_group) const;
    double ExcitedIntAverageFitness(const reparm::ParameterGroup &param_group) const;
    double ExcitedIntDiffFitness(const reparm::ParameterGroup &param_group) const;
    double FindSTDEV(const reparm::Fitness&,
		     const std::vector<reparm::ParameterGroup> &population,
		     const std::function<double (const reparm::ParameterGroup)>&);
		      
    
    Fitness(){};

  public:
    Fitness(const std::vector<reparm::ParameterGroup> &population,
            const std::vector<reparm::GaussianOutput> &high_level_outputs);

    std::string StringList(const reparm::ParameterGroup &param_group) const;
    double operator () (reparm::ParameterGroup &rhs) const;
    void operator () (std::vector<reparm::ParameterGroup> &rhs) const;
};

}

#endif /* FITNESS_H */
