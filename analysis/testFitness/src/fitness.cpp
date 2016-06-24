//
// Created by Dustin Tracy on 6/24/16.
//
/* This is the source file you were looking for.
Well at least most likely. This file contains the implimentations
of the objective function of the genetic algorithm.
Created by Dustin Tracy (dtracy.uf@gmail.com) April 11, 2016 */
#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>
#include <armadillo>
#include "../include/parameter_group.h"
#include "../include/fitness.h"
#include "../include/container_math.h"

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
    auto am1_differences = dmath::DifferenceVectorMatrix(am1_dipoles);
    std::vector<std::vector<double> > hlt_dipoles;
    for (auto &i: high_level_outputs_)
        hlt_dipoles.push_back(i.GetDipole());
    if (hlt_dipoles.empty())
        throw "Dipoles not found";
    auto hlt_differences = dmath::DifferenceVectorMatrix(hlt_dipoles);

    auto DistanceMatrix = dmath::DistanceVectorMatrix(am1_differences,
                                                      hlt_differences);
    return dmath::MatrixAverage(DistanceMatrix);

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
    auto am1_differences = dmath::DifferenceVectorMatrix(am1_es_freqs);
    std::vector<std::vector<double> > hlt_es_freqs;
    for (auto &i: high_level_outputs_)
        hlt_es_freqs.push_back(i.GetESFrequencies());
    auto hlt_differences = dmath::DifferenceVectorMatrix(hlt_es_freqs);

    auto DistanceMatrix = dmath::DistanceVectorMatrix(am1_differences,
                                                      hlt_differences);
    return dmath::MatrixAverage(DistanceMatrix);
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
    auto am1_differences = dmath::DifferenceVectorMatrix(am1_es_intensities);
    std::vector<std::vector<double> > hlt_es_intensities;
    for (auto &i: high_level_outputs_)
        hlt_es_intensities.push_back(i.GetESIntensities());
    if (am1_es_intensities.empty() || hlt_es_intensities.empty())
        throw "Cannot not obtain es_intensities";
    auto hlt_differences = dmath::DifferenceVectorMatrix(hlt_es_intensities);

    auto DistanceMatrix = dmath::DistanceVectorMatrix(am1_differences,
                                                      hlt_differences);
    return dmath::MatrixAverage(DistanceMatrix);
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
    auto am1_differences = dmath::DifferenceVectorMatrix(am1_ir_freqs);
    std::vector<std::vector<double> > hlt_ir_freqs;
    for (auto &i: high_level_outputs_)
        hlt_ir_freqs.push_back(i.GetFrequencies());
    auto hlt_differences = dmath::DifferenceVectorMatrix(hlt_ir_freqs);

    auto DistanceMatrix = dmath::DistanceVectorMatrix(am1_differences,
                                                      hlt_differences);
    return dmath::MatrixAverage(DistanceMatrix);
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
    auto am1_differences = dmath::DifferenceVectorMatrix(am1_ir_intensities);
    std::vector<std::vector<double> > hlt_ir_intensities;
    for (auto &i: high_level_outputs_)
        hlt_ir_intensities.push_back(i.GetIntensities());
    auto hlt_differences = dmath::DifferenceVectorMatrix(hlt_ir_intensities);
    if (am1_ir_intensities.empty() || hlt_ir_intensities.empty())
        throw "Cannot not obtain ir_intensities";

    auto DistanceMatrix = dmath::DistanceVectorMatrix(am1_differences,
                                                      hlt_differences);
    return dmath::MatrixAverage(DistanceMatrix);
}

double reparm::Fitness::ForceGeomDiffFitness
        (const reparm::ParameterGroup &param_group) const{
    /* Get the data */
    std::vector<std::vector<double> > am1_raw_forces;
    for (auto &i: param_group.GetOutputs())
        am1_raw_forces.push_back(i.GetForces());
    auto am1_differences = dmath::DifferenceVectorMatrix(am1_raw_forces);
    std::vector<std::vector<double> > hlt_raw_forces;
    for (auto &i: high_level_outputs_)
        hlt_raw_forces.push_back(i.GetForces());
    if (am1_raw_forces.size() < 2)
        throw "not enough geometries";
    auto hlt_differences = dmath::DifferenceVectorMatrix(hlt_raw_forces);

    auto DistanceMatrix = dmath::DistanceVectorMatrix(am1_differences,
                                                      hlt_differences);
    return dmath::MatrixAverage(DistanceMatrix);
}

reparm::Fitness::Fitness(const std::vector<reparm::ParameterGroup> &population,
                         const std::vector<reparm::GaussianOutput> &high_level_outputs)
        : high_level_outputs_{high_level_outputs}
        , energy_weight_{0.2}
        , dipole_average_weight_{0.5}
        , dipole_difference_weight_(0.3)
        , excited_freq_avg_weight_(14.0)
        , excited_freq_diff_weight_(0.6)
        , excited_int_avg_weight_(0.03)
        , excited_int_diff_weight_(0.13)
        , ir_freq_avg_weight_(55.0)
        , ir_freq_diff_weight_(200.0)
        , ir_int_avg_weight_(93.0)
        , ir_int_diff_weight_(25)
        , force_geom_diff_weight_(0.1)
        , force_geom_avg_weight_(1.0)
{}

std::string reparm::Fitness::StringList(const reparm::ParameterGroup &param_group) const{
    std::stringstream ss;

    try{
        double energy_fitness = EnergyFitness(param_group);
        ss << "Energy Fitness: ";
        ss << energy_fitness / energy_weight_ << std::endl;

        double dipole_average_fitness = DipoleAverageFitness(param_group);
        ss << "Dipole Average Fitness: ";
        ss << dipole_average_fitness / dipole_average_weight_ << std::endl;

        double dipole_difference_fitness = DipoleDifferenceFitness(param_group);
        ss << "Dipole Difference Fitness: ";
        ss << dipole_difference_fitness / dipole_difference_weight_ << std::endl;

        double excited_freq_average = ExcitedFreqAverageFitness(param_group);
        ss << "Excited State Frequency Average Fitness: ";
        ss << excited_freq_average / excited_freq_avg_weight_ << std::endl;

        double excited_freq_difference = ExcitedFreqDiffFitness(param_group);
        ss << "Excited State Frequency Differences Fitness: ";
        ss << excited_freq_difference / excited_freq_diff_weight_ << std::endl;

        double excited_int_average = ExcitedIntAverageFitness(param_group);
        ss << "Excited State Intensities Average Fitness: ";
        ss << excited_int_average / excited_int_avg_weight_ << std::endl;

        double excited_int_differences = ExcitedIntDiffFitness(param_group);
        ss << "Excited State Intensities Difference Fitness: ";
        ss << excited_int_differences / excited_int_diff_weight_ << std::endl;

        double ir_freq_average = IRFreqAverageFitness(param_group);
        ss << "IR Frequency Average Fitness: ";
        ss << ir_freq_average / ir_freq_avg_weight_ << std::endl;

        double ir_freq_difference = IRFreqDiffFitness(param_group);
        ss << "IR Frequency Differences Fitness: ";
        ss << ir_freq_difference / ir_freq_diff_weight_ << std::endl;

        double ir_int_average = IRIntensityAverageFitness(param_group);
        ss << "IR Intensities Average Fitness: ";
        ss << ir_int_average / ir_int_avg_weight_ << std::endl;

        double ir_int_differences = IRIntensityDiffFitness(param_group);
        ss << "IR Intensities Difference Fitness: ";
        ss << ir_int_differences / ir_int_diff_weight_ << std::endl;

        double force_geom_differences = ForceGeomDiffFitness(param_group);
        ss << "Force Geometry Difference Fitness: ";
        ss << force_geom_differences / force_geom_diff_weight_ << std::endl;

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
                e_fitness / energy_weight_
                + d_fitness / dipole_average_weight_
                + dd_fitness / dipole_difference_weight_
                + efa_fitness / excited_freq_avg_weight_
                + efd_fitness / excited_freq_diff_weight_
                + eia_fitness / excited_int_avg_weight_
                + eid_fitness / excited_int_diff_weight_
                + irfa_fitness / ir_freq_avg_weight_
                + irfd_fitness / ir_freq_diff_weight_
                + iria_fitness / ir_int_avg_weight_
                + irid_fitness / ir_int_diff_weight_
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
                    e_fitness / energy_weight_
                    + d_fitness / dipole_average_weight_
                    + dd_fitness / dipole_difference_weight_
                    + efa_fitness / excited_freq_avg_weight_
                    + efd_fitness / excited_freq_diff_weight_
                    + eia_fitness / excited_int_avg_weight_
                    + eid_fitness / excited_int_diff_weight_
                    + irfa_fitness / ir_freq_avg_weight_
                    + irfd_fitness / ir_freq_diff_weight_
                    + iria_fitness / ir_int_avg_weight_
                    + irid_fitness / ir_int_diff_weight_
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
