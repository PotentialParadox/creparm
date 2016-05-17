#include <genetic_algorithm.h>
#include <gaussian_input.h>
#include <iostream>
#include <fstream>
#include <gaussian.h>
#include <reparm_input.h>
#include <header.h>
#include <regex>
#include <sstream>
#include <system_tools.hpp>

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

/* This creates an input file that will
make an output file, from whos data
we will extract the coordinates and
the normal modes */
reparm::GaussianInput CreateHLTInput
(std::string &s, reparm::ReparmInput &reparm_input){
  std::ifstream fin(s);
  std::string line;
  std::getline(fin, line);
  std::stringstream input_file;
  input_file << "%chk=hlt" << std::endl;
  input_file << "#P " << reparm_input.GetHighLevelTheory();
  input_file << " opt" << std::endl;
  while (getline(fin, line))
    input_file << line + "\n";
  fin.close();
  auto gaus_input1 = reparm::GaussianInput(input_file.str());
  auto gaus_input2 = gaus_input1;
  std::stringstream header_ss;
  header_ss << "#P " << reparm_input.GetHighLevelTheory();
  header_ss << " freq\n\nhi" << std::endl;
  auto gaus_header = reparm::Header(header_ss.str());
  gaus_input2.SetHeader(gaus_header);
  gaus_input1.Link(gaus_input2);
  return reparm::GaussianInput(gaus_input1.str());
}

reparm::Coordinates FindOptCoordinates(const std::string &s){
  
}

reparm::GaussianInput reparm::CreateReparmGaussian
(std::string s, reparm::ReparmInput &reparm_input){
  /* We need to first optimize at the high
     level of theory and calculate the normal modes */
  auto hlt_input = CreateHLTInput(s, reparm_input);
  std::string hlt_output;
  std::string cmd{"#!/bin/sh\ng09 2>&1 <<END\n" + hlt_input.str() + "END"};
  std::string gout(systls::exec(cmd, 100000));
  std::regex p_normal_termination{"Normal termination of Gaussian 09"};
  std::regex p_no_g09{"g09: not found"};
  if (!std::regex_search(gout, p_normal_termination))
    throw "Initial HLT calculation failed";

  /* Now we need to parse this hlt output for the
     coordinates and normal modes */
}

std::string reparm::PrintTitle(){
  std::stringstream ss;
  ss << "/////////////////////////////////////////" << std::endl;
  ss << "//                                     //" << std::endl;
  ss << "//         AM1 Reparameterization      //" << std::endl;
  ss << "//             Dustin Tracy            //" << std::endl;
  ss << "//                 2016                //" << std::endl;
  ss << "//                                     //" << std::endl;
  ss << "/////////////////////////////////////////" << std::endl;
  ss << "\n" << std::endl;
  return ss.str();
}
  
