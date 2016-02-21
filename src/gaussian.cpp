#include <gaussian.h>
#include <iostream>
#include <string>
#include <regex>
#include <system_tools.hpp>
#include <reparm_exceptions.h>
#include <vector>
#include <thread>

using namespace reparm;

void reparm::Gaussian::ThreadRun(int j){
  // There will be as many outputs as there are inputs
  int number_inputs = static_cast<int>(this->param_group_.GetInputs().size());
  std::vector<std::string> outputs(number_inputs);
  std::cout << j << std::endl;
  try{
  for (auto i: this->param_group_.GetInputs()){
    std::string cmd{"#!/bin/sh\ng09 2>&1 <<END\n" + i.str() + "END"};
    std::string gout(systls::exec(cmd, 10000));
    std::regex p_normal_termination{"Normal termination of Gaussian 09"};
    std::regex p_no_g09{"g09: not found"};
    if (std::regex_search(gout, p_normal_termination)){
      outputs.push_back(gout);
    }
    else if (std::regex_search(gout, p_no_g09)){
      std::cerr << "Gaussian not found, please check your exports" << std::endl;
      gaussian_critical_error e;
      throw e;
    }
    else{
      std::cerr << "Gaussian Failure" << std::endl;
      gaussian_error e;
      throw e;
    }
  }
  } catch(const char *e){
    std::cout << e << std::endl;
    gaussian_error ge;
    throw ge;
  } catch(gaussian_critical_error e){
    throw e;
  }
}

void ThreadPrint(){
  std::cout << "hi" << std::endl;
}

std::vector<std::string> Gaussian::RunGaussian(){
  std::vector<std::string> out;
  return out;
}
