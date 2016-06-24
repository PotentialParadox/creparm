//
// Created by Dustin Tracy on 6/24/16.
//
#include <iostream>
#include <regex>
#include <sstream>
#include "../include/gaussian_output.h"
#include "../include/gout_reader.h"

reparm::GaussianOutput::GaussianOutput(const std::string &output_string){
    try{
        energy_ = reparm::gaussian::FindEnergy(output_string);
        es_frequencies_ = reparm::gaussian::FindESFrequencies(output_string);
        es_intensities_ = reparm::gaussian::FindESIntensities(output_string);
        dipole_ = reparm::gaussian::FindDipole(output_string);
        forces_ = reparm::gaussian::FindForces(output_string);
        frequencies_ = reparm::gaussian::FindFrequencies(output_string);
        intensities_ = reparm::gaussian::FindIntensities(output_string);
    }
    catch(const char *e){
        std::cout << e << std::endl;
        std::cerr << "There was a problem reading a gaussian output" << std::endl;
        throw;
    }
}

std::string reparm::GaussianOutput::str() const{
    std::stringstream ss;
    ss << "*****Printing Gaussian Output*****\n";
    ss << "Energy:\t\t" << this->energy_ << "\n";
    ss << "Excited States Frequencies:\t";
    for (auto i: es_frequencies_){ ss << i << " ";}
    ss << "\n";
    ss << "Excited States Intensities:\t";
    for (auto i: es_intensities_){ ss << i << " ";}
    ss << "\n";
    ss << "Dipole:\t\t";
    for (auto i: dipole_){ ss << i << " ";}
    ss << "\n";
    ss << "Forces:" << std::endl;
    for (size_t i = 0; i != forces_.size(); i+=3)
        ss << i/3 << "\t\t\t" << forces_[i] << "\t" << forces_[i+1]
        << "\t" << forces_[i+2] << std::endl;
    ss << "\n";
    ss << "IR Frequencies:\t";
    for (auto i: frequencies_){ ss << i << " ";}
    ss << "\n";
    ss << "IR Intensities:\t";
    for (auto i: intensities_){ ss << i << " ";}
    ss << "\n";
    return ss.str();
}

