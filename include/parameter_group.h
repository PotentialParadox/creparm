#ifndef PARAMETER_GROUP_H
#define PARAMETER_GROUP_H

#include <vector>
#include <gaussian_input.h>

namespace reparm{

class ParameterGroup{
  private:
    std::vector<reparm::GaussianInput> inputs_;
    std::vector<std::string> outputs_;

  public:
    ParameterGroup(const std::vector<reparm::GaussianInput> inputs)
      : inputs_{inputs}
    {}

    void SetInputs(const std::vector<reparm::GaussianInput>);
    std::vector<reparm::GaussianInput> GetInputs();
    void Mutate(const double &perturbation, const double &rate);
};

}

#endif /*  PARAMETER_GROUP_H */
