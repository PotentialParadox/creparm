#include <reparm_data.h>
#include <system_tools.hpp>
#include <gaussian_input.h>
#include <header.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <gaussian.h>
#include <fstream>

ReparmInput reparm::ReparmData::GetReparmInput() const{
  return reparm_input_;
}

const std::vector<reparm::GaussianOutput>& reparm::ReparmData::GetHighLevelOutputs() const{
  return high_level_outputs_;
}

std::vector<reparm::ParameterGroup> 
reparm::ReparmData::CreatePopulation(reparm::GaussianInput &input){
  // First perturb the coordinates
  int ng = reparm_input_.GetNumberGeometries();
  std::stringstream ss;
  ss << reparm_input_.GetNumberExcitedStates();
  std::string ne = ss.str();
  reparm::Header header{"#P AM1(Input,Print) CIS(Singlets,NStates=" + ne + ") pop(full) freq\n\nhi\n"};
  double pert = reparm_input_.GetGeometricPerturbation();
  std::vector<reparm::GaussianInput> inputs;
  for (int i = 0; i != ng; ++i){
    reparm::GaussianInput in = input;
    in.SetHeader(header);
    in.PerturbCoordinates(pert);
    inputs.push_back(in);
  }
  int ps = reparm_input_.GetPopulationSize();
  for (int i = 0; i != ps; ++i){
    population_.push_back(ParameterGroup{inputs});
  }
  return population_;
}

void reparm::ReparmData::CalculateHighLevel(){
  std::string hlt = reparm_input_.GetHighLevelTheory();
  std::stringstream ss;
  ss << reparm_input_.GetNumberExcitedStates();
  std::string ne{ss.str()};
  reparm::Parameters empty_params{""};
  reparm::Header first_header{"#P " + hlt + " CIS(Singlets,NStates=" + ne + ") pop(full)\n\nhi\n"};
  reparm::Header second_header{"#P " + hlt + " freq\n\nhi\n"};
  reparm::ParameterGroup hlgroup{population_[0]};
  std::vector<reparm::GaussianInput> inputs{hlgroup.GetInputs()};
  for (auto &&i: inputs){
    i.SetHeader(first_header);
    i.SetParameters(empty_params);
    reparm::GaussianInput freq{i};
    freq.SetHeader(second_header);
    i.Link(freq);
  }
  hlgroup.SetInputs(inputs);
  reparm::Gaussian gaussian{hlgroup};
  try{
    high_level_outputs_ = gaussian.RunGaussian();
  }
  catch(...){
    throw "Problem running high level calculations";
  }
}

void reparm::ReparmData::RunBest(){
  reparm::GaussianInput input1{population_[0].GetInputs()[0]};
  reparm::Header header1{"\%chk=best\n#P AM1(Input,Print) opt\n\nbest\n"};
  input1.SetHeader(header1);
  
  reparm::GaussianInput input2{input1};
  reparm::Header header2{"\%chk=best\n#P AM1(Input,Print) CIS(Singlets,NStates=" +
                         std::to_string(GetReparmInput().GetNumberExcitedStates()) +
                         ") pop(full) freq geom=Checkpoint\n\nbest1\n"};
  input2.SetHeader(header2);

  input1.Link(input2);
  std::ofstream fout{"best.com"};
  fout << input1.str();
  fout.close();
  std::string command{"g09 best.com best.log"};
  systls::exec(command, 100000);
}
