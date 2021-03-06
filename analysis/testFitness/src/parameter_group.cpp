//
// Created by Dustin Tracy on 6/24/16.
//
#include <sstream>
#include "../include/parameter_group.h"

void reparm::ParameterGroup::SetInputs(const std::vector<reparm::GaussianInput> inputs){
    this->inputs_ = inputs;
}

std::vector<reparm::GaussianInput> reparm::ParameterGroup::GetInputs() const{
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

reparm::Parameters reparm::ParameterGroup::GetParameters() const {
    return inputs_[0].GetParameters();
}

void reparm::ParameterGroup::SetParameters(const reparm::Parameters &parameters){
    for (auto &&i: inputs_)
        i.SetParameters(parameters);
}

reparm::ParameterGroup reparm::ParameterGroup::Cross(const reparm::ParameterGroup &rhs){
    reparm::Parameters params = inputs_[0].Cross(rhs.GetInputs()[0].GetParameters());
    reparm::ParameterGroup child = rhs;
    for (auto &&i: child.inputs_)
        i.SetParameters(params);
    return child;
}

void reparm::ParameterGroup::SetOutputs(const std::vector<reparm::GaussianOutput> &outputs){
    this->outputs_ = outputs;
}

void reparm::ParameterGroup::SetUpdated(const bool updated){
    updated_ = updated;
}

bool reparm::ParameterGroup::IsUpdated() const {
    return updated_;
}

void reparm::ParameterGroup::SetFitness(double fitness){
    fitness_ = fitness;
}

double reparm::ParameterGroup::GetFitness() const{
    return fitness_;
}

std::vector<reparm::GaussianOutput> reparm::ParameterGroup::GetOutputs() const{
    return this->outputs_;
}

std::string reparm::ParameterGroup::XYZString() const{
    std::string s;
    for (const auto &i: inputs_)
        s += i.GetCoordinates().XYZString();
    return s;
}
