#include <parameter_group.h>
#include <parameters.h>

void reparm::ParameterGroup::SetInputs(const std::vector<reparm::GaussianInput> inputs){
  this->inputs_ = inputs;
}

std::vector<reparm::GaussianInput> reparm::ParameterGroup::GetInputs(){
  return this->inputs_;
}

void reparm::ParameterGroup::Mutate(const double &perturbation, const double &rate){
  // Parameters among the input files must be the same so we only use 
  // the parameters from the first input file
  reparm::Parameters parameters = this->inputs_[0].GetParameters();
  parameters.Mutate(perturbation, rate);
  // Now we set all the member input file's parameters to this
  for (auto &&i: this->inputs_){
    i.SetParameters(parameters);
  }
}
