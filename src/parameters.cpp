#include <parameters.h>
#include <iostream>
#include <sstream>
#include <regex>
#include <chrono>
#include <random>

reparm::Parameters::Parameters(const std::string &s){
  try{
    ReadLabels(s);
    ExtractFloats();
  }
  catch(...){
    std::cerr << "Was unable to read parameters" << std::endl;
    throw "Error";
  }
}

void reparm::Parameters::ReadLabels(const std::string &s){ ;
  std::regex p_label{"(\\S+\\s)|\n"};
  std::sregex_iterator pos(s.cbegin(), s.cend(), p_label);
  std::sregex_iterator end;
  std::vector<std::string> labels;
  for (; pos != end; ++pos){
    labels.push_back(pos->str(0));
  }
  this->parameter_labels_ = labels;
}

void reparm::Parameters::SetParameters(const std::vector<double> p_floats){
  this->parameter_floats_ = p_floats;
}

void reparm::Parameters::ExtractFloats(){
  std::regex p_float{"-?\\d+\\.\\d+"};  // Used to extract the floats 
  std::vector<double> p_floats;
  std::regex p_not_label{"-?\\d+\\.\\d+.*\n*"};  // Used to extract the non floats
  std::regex p_eisol{"EISol"};
  std::regex p_eheat{"EHeat"};
  std::regex p_newline{"\n"};
  int line_count = 0;
  for (size_t i = 0; i != this->parameter_labels_.size(); ++i){
    // Begin the parameters to ignore
    if (std::regex_search((this->parameter_labels_)[i], p_eisol)){
      continue;
    }
    if (std::regex_search((this->parameter_labels_)[i], p_eheat)){
      continue;
    }
    // End the parameters to ignore
    std::sregex_iterator pos(this->parameter_labels_[i].cbegin(),
                             this->parameter_labels_[i].cend(), p_float);
    std::sregex_iterator end;
    int pos_count = 0;
    for (; pos != end; ++pos){
      p_floats.push_back(std::stod(pos->str(0)));
      p_floats.push_back(i);
      p_floats.push_back(pos_count);
      p_floats.push_back(line_count);
      ++pos_count;
    }
    if (std::regex_search(this->parameter_labels_[i], p_newline)){
      line_count++;
    }
    // Erase the floats from the label
    this->parameter_labels_[i] = std::regex_replace(this->parameter_labels_[i], p_not_label, "");
  }
  this->parameter_floats_ = p_floats;
}

std::string reparm::Parameters::str() const{
  std::vector<std::string> parameters = this->parameter_labels_;
  std::string parameter_string;
  std::vector<double> const *p = &this->parameter_floats_;
  for (size_t i = 0; i < (*p).size(); i+=4){
    std::stringstream ss;
    ss.setf(std::ios::fixed);
    ss.precision(10);
    if ((*p)[i+2] == 0){
      ss << (*p)[i];
    }
    else{
      ss << "," << (*p)[i];
    }
    if (i >= (*p).size() - 4){
      ss << "\n";
    }
    else if ((*p)[i+7] > (*p)[i+3]){
      ss << "\n";
    }
    else if ((*p)[i+1] < (*p)[i+5]){
      ss << " ";
    }
    parameters[(*p)[i+1]] += ss.str();
  }
  for (auto i: parameters){parameter_string += i;}
  return parameter_string;
}

void reparm::Parameters::Mutate(const double  &perturbation, const float &rate){
  try{
    std::vector<double> *p = &this->parameter_floats_;
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine rand(seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (size_t i = 0; i < (*p).size(); i+=4){
      // Determine if parameter is mutated
      if (distribution(rand) < rate){
        double stdev = (*p)[i] * perturbation;
        std::normal_distribution<double> p_distribution((*p)[i], stdev);
        (*p)[i] = p_distribution(rand);
      }
    }
  }
  catch(...){
    std::cerr << "Uknown error with mutation" << std::endl;
    throw "Error";
  }
}

reparm::Parameters reparm::Parameters::Cross(const reparm::Parameters& rhs){
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine rand(seed);
    std::uniform_int_distribution<int> distribution(0, 1);
    reparm::Parameters cross_parameters = rhs;
    std::vector<double> cross_p_floats;
    for (size_t i = 0; i < parameter_floats_.size(); ++i){
      int r = distribution(rand);
      if (r){
        cross_p_floats.push_back(parameter_floats_[i]);
      }else{
        cross_p_floats.push_back(rhs.parameter_floats_[i]);
      }
    }
    cross_parameters.SetParameters(cross_p_floats);
    return cross_parameters;
}

bool reparm::Parameters::empty() const{
  return parameter_floats_.empty();
}