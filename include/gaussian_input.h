/* This Gaussian Input class is in charge of building and reading 
 * Gaussian input files
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 2016 
 */

#ifndef GAUSSIAN_INPUT_H
#define GAUSSIAN_INPUT_H

#include <string>
#include <header.h>
#include <coordinates.h>
#include <parameters.h>
#include <header.h>
#include <coordinates.h>
#include <parameters.h>
#include <reparm_exceptions.h>

namespace reparm{

class GaussianInput{
  private:
    reparm::Header header_;
    reparm::Coordinates coordinates_;
    reparm::Parameters parameters_;

  public:
    GaussianInput(const std::string&);

    std::string ReadFile(const std::string&) const;
    reparm::Coordinates ReadCoordinates(const std::string&);
    reparm::Header ReadHeader(const std::string&);
    reparm::Parameters ReadParameters(const std::string&);
    void SetHeader(const reparm::Header&);
    void SetCoordinates(const reparm::Coordinates&);
    void PerturbCoordinates(const float &p);
    void SetParameters(const reparm::Parameters&);
    void MutateParameters(const double &p, const float &r);
    reparm::Header GetHeader() const;
    reparm::Coordinates GetCoordinates() const;
    reparm::Parameters GetParameters() const;
    std::string str();
};

}

#endif /* GAUSSIAN_INPUT_H */
