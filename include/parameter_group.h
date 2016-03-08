#ifndef PARAMETER_GROUP_H
#define PARAMETER_GROUP_H

#include <vector>
#include <gaussian_input.h>
#include <gaussian_output.h>

namespace reparm{

class ParameterGroup{
  private:
    std::vector<reparm::GaussianInput> inputs_;
    std::vector<reparm::GaussianOutput> outputs_;

  public:
    ParameterGroup(const std::vector<reparm::GaussianInput> inputs)
      : inputs_{inputs}
    {}
    ParameterGroup()
      : inputs_{}
      , outputs_{}
    {}

    void SetInputs(const std::vector<reparm::GaussianInput>);
    std::vector<reparm::GaussianInput> GetInputs();
    void SetOutputs(const std::vector<reparm::GaussianOutput>&);
    std::vector<reparm::GaussianOutput> GetOutputs();
    void Mutate(const double &perturbation, const double &rate);
};

}

#endif /*  PARAMETER_GROUP_H */
