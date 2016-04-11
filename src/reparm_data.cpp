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
  reparm::Parameters empty_params{""};
  reparm::Header first_header{"#P AM1(Input,Print) CIS(Singlets,NStates=" + ne + ") pop(full)\n\nhi\n"};
  reparm::Header second_header{"#P AM1(Input, Print) freq\n\nhi\n"};
  double pert = reparm_input_.GetGeometricPerturbation();
  std::vector<reparm::GaussianInput> inputs;
  for (int i = 0; i != ng; ++i){
    reparm::GaussianInput input_es = input;
    input_es.SetHeader(first_header);
    input_es.PerturbCoordinates(pert);
    reparm::GaussianInput input_freq{input_es};
    input_freq.SetHeader(second_header);
    input_es.Link(input_freq);
    inputs.push_back(input_es);
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
  reparm::GaussianInput input_freq{population_[0].GetInputs()[0]};
  reparm::GaussianInput input_es{population_[0].GetInputs()[0]};
  reparm::Header header1{"\%chk=best\n#P AM1(Input,Print) opt\n\nbest\n"};
  input_freq.SetHeader(header1);
  
  /* Create an input file for caculating frequencies */
  reparm::GaussianInput input2{input_freq};
  reparm::Header header2{"\%chk=best\n#P AM1(Input,Print) freq geom=Checkpoint\n\nbest1\n"};
  input2.SetHeader(header2);
  input2.ClearCoordinates();
  input_freq.Link(input2);
  std::ofstream fout{"best_freq.com"};
  fout << input_freq.str();

  /* Create an input file for caculating excited_states */
  reparm::GaussianInput input3{input_freq};
  reparm::Header header3{"\%chk=best\n#P AM1(Input,Print) CIS(Singlets,NStates=" +
                         std::to_string(GetReparmInput().GetNumberExcitedStates()) +
                         ") pop(full) geom=Checkpoint\n\nbest1\n"};
  input3.SetHeader(header3);
  input3.ClearCoordinates();
  input_freq.Link(input3);
  std::ofstream fout_es{"best_es.com"};
  fout_es << input_freq.str();

  
  fout.close();
  fout_es.close();
  std::string command{"g09 best_freq.com best_freq.log"};
  systls::exec(command, 100000);
  command = "g09 best_es.com best_es.log";
  systls::exec(command, 100000);
}
