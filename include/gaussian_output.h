/* This gaussian output class reads and stores the information
 * produced by gaussian 09. 
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 2016
 */

#ifndef GAUSSIAN_OUTPUT_H
#define GAUSSIAN_OUTPUT_H

#include <vector>
#include <string>

namespace reparm{

class GaussianOutput{
  private:
    double energy_;
    std::vector<double> dipole_;
    std::vector<double> excited_states_;
    std::vector<double> forces_;
    std::vector<double> frequencies_;
    std::vector<double> intensities_;

  public:
    GaussianOutput()
      : energy_{0}
      , dipole_{0}
      {}

    GaussianOutput(const std::string&);

    std::string str();
};

}

#endif /* GAUSSIAN_OUTPUT_H */