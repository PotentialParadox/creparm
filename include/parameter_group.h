/* Parameter group objects are treated as the indivuals in a genetic algorithm.
In this program, the parameter values are treated as genes. 
Since this class is a collection of all inputs with the same gene,
treating it as an indivual made since.
Create by Dustin Tracy (dtracy.uf@gmail.com) April 11, 2016 */
#ifndef PARAMETER_GROUP_H
#define PARAMETER_GROUP_H

#include <vector>
#include <parameters.h>
#include <gaussian_input.h>
#include <gaussian_output.h>

namespace reparm{

class ParameterGroup{
  private:
    std::vector<reparm::GaussianInput> inputs_;
    std::vector<reparm::GaussianOutput> outputs_;
    double fitness_;

  public:
    ParameterGroup(const std::vector<reparm::GaussianInput> inputs)
      : inputs_{inputs}
      , fitness_{1.0}
    {}
    ParameterGroup()
      : inputs_{}
      , outputs_{}
      , fitness_{1.0}
    {}

    void SetInputs(const std::vector<reparm::GaussianInput>);
    std::vector<reparm::GaussianInput> GetInputs() const;
    void SetOutputs(const std::vector<reparm::GaussianOutput>&);
    std::vector<reparm::GaussianOutput> GetOutputs() const;
    void SetFitness(double fitness);
    double GetFitness();
    void Mutate(const double &perturbation, const double &rate);
    reparm::ParameterGroup Cross(const reparm::ParameterGroup&);
    void SetParameters(const reparm::Parameters&);

    bool operator < (const reparm::ParameterGroup &rhs) const{
      return fitness_ < rhs.fitness_;
    }
};

}

#endif /*  PARAMETER_GROUP_H */
