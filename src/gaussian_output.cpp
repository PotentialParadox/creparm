#include <gaussian_output.h>
#include <iostream>
#include <regex>
#include <sstream>

double FindEnergy(const std::string &s){
  std::regex p_energy{"\n\\s*SCF\\s+Done.*"};
  std::smatch m;
  if(!std::regex_search(s, m, p_energy)){
    std::cerr << "Cannot find energies" << std::endl;
    throw "Error";
  }
  std::string e_line{m[0].str()};
  std::regex p_float{"-?\\d+\\.\\w+-?\\w*"};
  std::regex_search(e_line, m, p_float);
  return stod(m[0].str());
}

std::vector<double> FindDipole(const std::string &s){
  std::regex p_dipole{"Charge=.*\n.*Dipole\\s+moment.*\n.*"};
  std::sregex_iterator pos(s.begin(), s.end(), p_dipole);
  std::sregex_iterator end;
  std::vector<std::string> dipole_list;
  for (; pos != end; ++pos)
    dipole_list.push_back(pos->str(0));
  std::string last_dipole{*(dipole_list.end() - 1)};

  std::regex p_x{"X=\\s+(-?\\d+\\.\\d+)\\s+"};
  std::smatch m;
  std::regex_search(last_dipole, m, p_x);
  double x{stod(m[1])};

  std::regex p_y{"Y=\\s+(-?\\d+\\.\\d+)\\s+"};
  std::regex_search(last_dipole, m, p_y);
  double y{stod(m[1])};

  std::regex p_z{"Z=\\s+(-?\\d+\\.\\d+)\\s+"};
  std::regex_search(last_dipole, m, p_z);
  double z{stod(m[1])};

  return std::vector<double>{x, y, z};
}

std::vector<double> FindForces(const std::string &s){
  std::regex p_forces{"Forces\\s+\\(.*\n.*\n.*\n(\\s+\\d+\\s+\\d+\\s+.*\n)*"};
  std::smatch m;
  std::regex_search(s, m, p_forces);
  std::string force_string = m[0];
  std::regex p_floats{"-?\\d+\\.\\d+"};
  std::sregex_iterator pos1(force_string.cbegin(), force_string.cend(), p_floats);
  std::sregex_iterator end1;
  std::vector<double> forces;
  for (; pos1 != end1; ++pos1){
    forces.push_back(stod(pos1->str(0)));
  }
  if (forces.empty()) throw "Cannot find forces";
  return forces;
}

std::vector<double> FindFrequencies(const std::string &s){
  std::regex p_frequencies{"\n\\s+Frequencies.*-?\\d+\\.\\d+"};
  std::sregex_iterator pos(s.begin(), s.end(), p_frequencies);
  std::sregex_iterator end;
  std::string freq_string;
  for (; pos != end; ++pos){
    freq_string += pos->str(0);
  }
  std::regex p_floats{"-?\\d+\\.\\d+"};
  std::sregex_iterator pos1(freq_string.cbegin(), freq_string.cend(), p_floats);
  std::sregex_iterator end1;
  std::vector<double> frequencies;
  for (; pos1 !=end1; ++pos1){
    frequencies.push_back(stod(pos1->str(0)));
  }
  if (frequencies.empty()) throw "Cannot find frequencies";
  return frequencies;
}

std::vector<double> FindIntensities(const std::string &s){
  std::regex p_intensities{"\n\\s+IR\\s+Inten.*-?\\d+\\.\\d+"};
  std::sregex_iterator pos(s.begin(), s.end(), p_intensities);
  std::sregex_iterator end;
  std::string ir_string;
  for (; pos != end; ++pos){
    ir_string += pos->str(0);
  }
  std::regex p_floats{"-?\\d+\\.\\d+"};
  std::sregex_iterator pos1(ir_string.cbegin(), ir_string.cend(), p_floats);
  std::sregex_iterator end1;
  std::vector<double> intensities;
  for (; pos1 !=end1; ++pos1){
    intensities.push_back(stod(pos1->str(0)));
  }
  if (intensities.empty()) throw "Cannot find intensities";
  return intensities;
}

std::vector<double> FindESFrequencies(const std::string &s){
  std::regex p_excited{"\n\\s*Excited\\s+State.*(-?\\d+\\.\\d+)\\s+eV"};
  std::sregex_iterator pos(s.cbegin(), s.cend(), p_excited);
  std::sregex_iterator end;
  std::vector<double> es_freqs;
  for (; pos != end; ++pos){
    es_freqs.push_back(std::stod(pos->str(1)));
  }
  if (es_freqs.empty()) throw "cannot find es_freqs";
  return es_freqs;
}

std::vector<double> FindESIntensities(const std::string &s){
  std::regex p_excited{"\n\\s*Excited\\s+State.*f=(-?\\d+\\.\\d+)\\s"};
  std::sregex_iterator pos(s.cbegin(), s.cend(), p_excited);
  std::sregex_iterator end;
  std::vector<double> es_intensities;
  for (; pos != end; ++pos){
    es_intensities.push_back(std::stod(pos->str(1)));
  }
  if (es_intensities.empty()) throw "cannot find es_intensities";
  return es_intensities;
}

reparm::GaussianOutput::GaussianOutput(const std::string &output_string){
  try{
    this->energy_ = FindEnergy(output_string);
    this->es_frequencies_ = FindESFrequencies(output_string);
    this->es_intensities_ = FindESIntensities(output_string);
    this->dipole_ = FindDipole(output_string);
    this->forces_ = FindForces(output_string);
    this->frequencies_ = FindFrequencies(output_string);
    this->intensities_ = FindIntensities(output_string);
  }
  catch(...){
    std::cerr << "There was a problem reading a gaussian output" << std::endl;
    throw;
  }
}

std::string reparm::GaussianOutput::str() const{
  std::stringstream ss;
  ss << "*****Printing Gaussian Output*****\n";
  ss << "Energy:\t\t" << this->energy_ << "\n";
  ss << "Excited States Frequencies:\t";
  for (auto i: es_frequencies_){ ss << i << " ";}
  ss << "\n"; 
  ss << "Excited States Intensities:\t";
  for (auto i: es_intensities_){ ss << i << " ";}
  ss << "\n"; 
  ss << "Dipole:\t\t";
  ss << "Dipole:\t\t";
  for (auto i: dipole_){ ss << i << " ";}
  ss << "\n"; 
  ss << "Forces:\t\t";
  for (auto i: forces_){ ss << i << " ";}
  ss << "\n"; 
  ss << "IR Frequencies:\t";
  for (auto i: frequencies_){ ss << i << " ";}
  ss << "\n"; 
  ss << "IR Intensities:\t";
  for (auto i: intensities_){ ss << i << " ";}
  ss << "\n"; 
  return ss.str();
}

