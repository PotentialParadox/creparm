#ifndef GAUS_READER_H
#define GAUS_READER_H

#include <string>
#include <regex>
#include <iostream>
#include <coordinates.h>


namespace reparm{
  namespace gaussian{

    double FindEnergy(const std::string &s);
    std::vector<double> FindDipole(const std::string &s);
    std::vector<double> FindForces(const std::string &s);
    std::vector<double> FindFrequencies(const std::string &s);
    std::vector<double> FindIntensities(const std::string &s);
    std::vector<double> FindESFrequencies(const std::string &s);
    std::vector<double> FindESIntensities(const std::string &s);
    void FindOptCoordinates(const std::string &s);

  }
}

#endif /*GAUS_READER_H*/