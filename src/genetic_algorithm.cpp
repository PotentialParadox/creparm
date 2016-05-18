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
  /* We read the input from the user then convert
     it to an hlt input file for opt coordinates
     and frequencies */
  reparm::GaussianInput gaus_input1(s);
  std::stringstream header_ss;
  header_ss << "%chk=hlt" << std::endl;
  header_ss << "#P " << reparm_input.GetHighLevelTheory();
  header_ss << " opt\n" << std::endl;
  header_ss << "hi\n" << std::endl;
  reparm::Header hlt_header1(header_ss.str());
  gaus_input1.SetHeader(hlt_header1);
  auto gaus_input2 = gaus_input1;
  std::stringstream header_ss2;
  header_ss2 << "%chk=hlt" << std::endl;
  header_ss2 << "#P " << reparm_input.GetHighLevelTheory();
  header_ss2 << " freq geom=Checkpoint\n" << std::endl;
  header_ss2 << "hi\n" << std::endl;
  reparm::Header hlt_header2(header_ss2.str());
  gaus_input2.SetHeader(hlt_header2);
  gaus_input2.ClearCoordinates();
  gaus_input1.Link(gaus_input2);
  return gaus_input1;
}

reparm::Coordinates FindOptCoordinates(const std::string &s){
  
}

reparm::GaussianInput reparm::CreateReparmGaussian
(std::string s, reparm::ReparmInput &reparm_input){
  /* We need to first optimize at the high
     level of theory and calculate the normal modes */
  auto hlt_input = CreateHLTInput(s, reparm_input);
  std::cout << "Created HLT file" << std::endl;
  std::ofstream fout("test.com");
  fout << hlt_input.str();
  std::string cmd{"#!/bin/sh\ng09 2>&1 <<END\n" + hlt_input.str() + "END"};
  std::string hlt_out(systls::exec(cmd, 100000));
  std::regex p_normal_termination{"Normal termination of Gaussian 09"};
  std::regex p_no_g09{"g09: not found"};
  if (!std::regex_search(hlt_out, p_normal_termination))
    throw "Initial HLT calculation failed";
  std::ofstream hlt_output("test.log");
  hlt_output << hlt_out;

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
  
