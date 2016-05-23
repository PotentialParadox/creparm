/* This class was created to parse gaussian outputs 
   Created by Dustin Tracy (dtracy.ufl@gmail.com) */

#ifndef GAUS_READER_H
#define GAUS_READER_H

#include <string>
#include <regex>
#include <iostream>
#include <coordinates.h>
#include <parameters.h>


namespace reparm{
  namespace gaussian{

    double FindEnergy(const std::string &s);
    std::vector<double> FindDipole(const std::string &s);
    std::vector<double> FindForces(const std::string &s);
    std::vector<double> FindFrequencies(const std::string &s);
    std::vector<double> FindIntensities(const std::string &s);
    std::vector<double> FindESFrequencies(const std::string &s);
    std::vector<double> FindESIntensities(const std::string &s);
    reparm::Coordinates FindOptCoordinates(const std::string &s);
    reparm::Parameters FindParameters(const std::string &s);
    std::vector<std::vector<float> > FindNormalModes(const std::string &s);

  }
}

#endif /*GAUS_READER_H*/