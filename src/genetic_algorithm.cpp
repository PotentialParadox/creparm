#include <genetic_algorithm.h>
#include <gaussian_input.h>
#include <iostream>
#include <fstream>
#include <gaussian.h>
#include <header.h>
#include <regex>

std::string CreateInput(std::string &s){
  std::ifstream fin(s);
  std::string input_file;
  std::string line;
  std::getline(fin, line);
  input_file += "#P AM1(Input,print)\n";
  while (getline(fin, line)){
    input_file += line + "\n";
  }
  fin.close();
  return input_file;
}

reparm::GaussianInput reparm::CreateReparmGaussian(std::string s){
  GaussianInput g_input{s};
  if (g_input.GetParameters().str().empty()){
    std::string input{CreateInput(s)};
    Gaussian gaussian{};
    std::string output{gaussian.RunGaussian(input)};
    std::regex p_parameters{"(\\s+Method=(.|\n)*)Standard"};
    std::smatch m;
    regex_search(output, m, p_parameters);
    g_input.SetParameters(g_input.ReadParameters(m[1]));
  }
  return g_input;
}

