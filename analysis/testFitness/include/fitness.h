//
// Created by Dustin Tracy on 6/24/16.
//
/* This functor is in charge of calculating the cost function in the genetic algorithm.
 * This is most likely the class that you will want to modify.
 * Note that this updates the fitness members found in the ParameterGroup class
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 3/8/2016 */
#ifndef FITNESS_H
#define FITNESS_H

#include <vector>
#include <functional>
#include "gaussian_output.h"
#include "parameter_group.h"

namespace reparm{

    class Fitness{
        friend class Mutate;

    private:
        std::vector<reparm::GaussianOutput> high_level_outputs_;
        /* Energy Fitness */
        double energy_weight_;
        /* Dipole Average Fitness */
        double dipole_average_weight_;
        /* Dipole Difference Fitness */
        double dipole_difference_weight_;
        /* Exctied State Frequencies average */
        double excited_freq_avg_weight_;
        /* Excited State Frequencies differences */
        double excited_freq_diff_weight_;
        /* Exctied State Intensities average */
        double excited_int_avg_weight_;
        /* Excited State Intensities differences */
        double excited_int_diff_weight_;
        /* Infrared frequencies average */
        double ir_freq_avg_weight_;
        /* Infrared frequencies differences */
        double ir_freq_diff_weight_;
        /* Infrared intensities average */
        double ir_int_avg_weight_;
        /* Infrared intensities differences */
        double ir_int_diff_weight_;
        /* The force difference between atoms of differenct geometries */
        double force_geom_diff_weight_;
        /* The average force for each atom among the geometriees */
        double force_geom_avg_weight_;
        double EnergyFitness(const reparm::ParameterGroup &param_group) const;
        double DipoleAverageFitness(const reparm::ParameterGroup &param_group) const;
        double DipoleDifferenceFitness(const reparm::ParameterGroup &param_group) const;
        double ExcitedFreqAverageFitness(const reparm::ParameterGroup &param_group) const;
        double ExcitedFreqDiffFitness(const reparm::ParameterGroup &param_group) const;
        double ExcitedIntAverageFitness(const reparm::ParameterGroup &param_group) const;
        double ExcitedIntDiffFitness(const reparm::ParameterGroup &param_group) const;
        double IRFreqAverageFitness(const reparm::ParameterGroup &param_group) const;
        double IRFreqDiffFitness(const reparm::ParameterGroup &param_group) const;
        double IRIntensityAverageFitness(const reparm::ParameterGroup &param_group) const;
        double IRIntensityDiffFitness(const reparm::ParameterGroup &param_group) const;
        double ForceGeomDiffFitness(const reparm::ParameterGroup &param_group) const;
        double ForceGeomAvgFitness(const reparm::ParameterGroup &param_group) const;
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
