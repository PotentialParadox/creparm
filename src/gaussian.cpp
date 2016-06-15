#include <gaussian.h>
#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <system_tools.hpp>
#include <reparm_exceptions.h>
#include <vector>
#include <thread>
#include <gaussian_output.h>
#include <fstream>

using namespace reparm;

std::string reparm::Gaussian::RunGaussian(std::string &input_file){
  std::string cmd{"#!/bin/sh\ng09 2>&1 <<END\n" + input_file + "END"};
  std::string gout(systls::exec(cmd, 100000));
  std::regex p_normal_termination{"Normal termination of Gaussian 09"};
  std::regex p_no_g09{"g09: not found"};
  if (std::regex_search(gout, p_no_g09)){
    std::cerr << "Gaussian not found, please check your exports" << std::endl;
    throw "No Gaussian";
  }
  else if (!(std::regex_search(gout, p_normal_termination))){
    throw "There was an error with gaussian";
  }
  return gout;
}

void ThreadRun(int j, reparm::ParameterGroup param_group,
               std::vector<reparm::GaussianOutput> &outputs, bool &failure){
// There will be as many outputs as there are inputs
int number_inputs = static_cast<int>(param_group.GetInputs().size());
int number_threads = std::thread::hardware_concurrency();
  for (int i = j; i < number_inputs; i += number_threads){
    std::string cmd{"#!/bin/sh\ng09 2>&1 <<END\n" + param_group.GetInputs()[i].str() + "END"};
    std::string gout(systls::exec(cmd, 100000));
    std::regex p_normal_termination{"Normal termination of Gaussian 09"};
    std::regex p_no_g09{"g09: not found"};
    if (std::regex_search(gout, p_normal_termination)){
      reparm::GaussianOutput output{gout};
      outputs[i] = output;
    }
    else if (std::regex_search(gout, p_no_g09)){
      std::cerr << "Gaussian not found, please check your exports" << std::endl;
      throw "No Gaussian";
    }
    else{
      std::cout << "Gaussian failed to run to completion" << std::endl;
      std::stringstream ss;
      ss << "g_failure_" << j << ".log";
      std::ofstream fout{ss.str()};
      fout << gout << std::endl;
      failure = true;
    }
  }
}

void do_join(std::thread &t){
  t.join();
}

void join_all(std::vector<std::thread> &v){
  std::for_each(v.begin(), v.end(), do_join);
}

std::vector<reparm::GaussianOutput> Gaussian::RunGaussian(){
  int number_inputs = this->param_group_.GetInputs().size();
  std::vector<reparm::GaussianOutput> outputs(number_inputs);
  reparm::ParameterGroup param_group = this->param_group_;
  std::vector<std::thread> thread_list;
  int number_threads = std::thread::hardware_concurrency();
  bool gaussian_failure = false;
  try{
    for (int i = 1; i < number_threads; ++i){
      thread_list.push_back(std::thread(ThreadRun, i, param_group, std::ref(outputs),
                                        std::ref(gaussian_failure)));
    }
    ThreadRun(0, param_group, outputs, gaussian_failure);
    join_all(thread_list);
    if (gaussian_failure == true){
      reparm::gaussian_error e;
      throw e;
    }
  }
  catch(...){
    throw;
  }

  return outputs;
}

std::vector<reparm::GaussianOutput> Gaussian::RunGaussian(reparm::ParameterGroup& param_group){
  int number_inputs = param_group.GetInputs().size();
  std::vector<reparm::GaussianOutput> outputs(number_inputs);
  std::vector<std::thread> thread_list;
  int number_threads = std::thread::hardware_concurrency();
  bool gaussian_failure = false;
  try{
    for (int i = 1; i < number_threads; ++i){
      thread_list.push_back(std::thread(ThreadRun, i, param_group, std::ref(outputs),
                                        std::ref(gaussian_failure)));
    }
    ThreadRun(0, param_group, outputs, gaussian_failure);
    join_all(thread_list);
    if (gaussian_failure == true){
      reparm::gaussian_error e;
      throw e;
    }
  }
  catch(...){
    throw;
  }

  return outputs;
}

reparm::GaussianOutput Gaussian::RunGaussian(reparm::GaussianInput &input){
  std::string s_input = input.str();
  std::string s_output = g_run.RunGaussian(s_input);
  return GaussianOutput(s_output);
}

reparm::Gaussian g_run = reparm::Gaussian();