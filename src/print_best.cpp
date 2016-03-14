#include <print_best.h>
#include <fstream>
#include <gaussian_input.h>
#include <header.h>

reparm::PrintBest::PrintBest(reparm::ReparmData &reparm_data)
  : reparm_data_{reparm_data}
  , number_excited_states_{reparm_data.GetReparmInput().GetNumberExcitedStates()}
 {}

void reparm::PrintBest::operator()(){
  reparm::GaussianInput input1{reparm_data_.population_[0].GetInputs()[0]};
  reparm::Header header1{"#P AM1(Input,Print) opt\n\nbest\n"};
  input1.SetHeader(header1);
  
  reparm::GaussianInput input2{input1};
  reparm::Header header2{"#P AM1(Input,Print) CIS(Singlets,NStates=" +
                         std::to_string(number_excited_states_) +
                         ") pop(full) freq\n\nbest1\n"};
  input2.SetHeader(header2);

  input1.Link(input2);
  std::ofstream fout{"best.com"};
  fout << input1.str();
  fout.close();
}
