#include <gaussian_input.h>
#include <regex>
#include <iostream>
#include <fstream>


reparm::GaussianInput::GaussianInput(const std::string &s){
  try{
  std::string file{ReadFile(s)};
  reparm::Header header{ReadHeader(file)};
  this->header_.push_back(header);
  reparm::Coordinates coordinates{ReadCoordinates(file)};
  this->coordinates_.push_back(coordinates);
  reparm::Parameters parameters{ReadParameters(file)};
  this->parameters_.push_back(parameters);
  }
  catch(...){
    std::cerr << "Gaussian Input Read Error" << std::endl;
    gaussian_input_error e;
    throw e;
  }
}

std::string reparm::GaussianInput::ReadFile(const std::string &file_name) const{
  using namespace std;
  try{
    string file;
    string line;
    ifstream fin{file_name};
    if (!fin){
      cerr << "Unable to open " + file_name << endl;
      throw "Unable to open";
    }
    while(getline(fin, line)){
      file += line + "\n";
    }
    fin.close();
    return file;
  }
  catch(const char *e){
    throw e;
  }
}

reparm::Header reparm::GaussianInput::ReadHeader(const std::string &file){
  using namespace std;
  regex p_header{"^(.*\n){3}"};
  smatch m;
  regex_search(file, m, p_header);
  reparm::Header header{m[0]};
  return header;
}

reparm::Coordinates reparm::GaussianInput::ReadCoordinates(const std::string &file){
  using namespace std;
  regex p_coordinates{"\n\\s*\\d+\\s+\\d+\\s*\n(\\s*\\S+\\s+-?\\d+\\..+)+"};
  smatch m;
  regex_search(file, m, p_coordinates);
  reparm::Coordinates coordinates{m[0]};
  return coordinates;
}

reparm::Parameters reparm::GaussianInput::ReadParameters(const std::string &file){
  using namespace std;
  regex p_parameters{"Method=(.|\n)*$"};
  smatch m;
  regex_search(file, m, p_parameters);
  reparm::Parameters parameters{m[0]};
  return parameters;
}

void reparm::GaussianInput::SetHeader(const reparm::Header &header){
  this->header_[0] = header;
}

void reparm::GaussianInput::SetCoordinates(const reparm::Coordinates &coordinates){
  this->coordinates_[0] = coordinates;
}

void reparm::GaussianInput::SetParameters(const reparm::Parameters &parameters){
  this->parameters_[0] = parameters;
}

void reparm::GaussianInput::PerturbCoordinates(const float &p){
  this->coordinates_[0].Perturb(p);
}

void reparm::GaussianInput::MutateParameters(const double &p, const float &r){
  this->parameters_[0].Mutate(p, r);
}

reparm::Parameters reparm::GaussianInput::Cross(const reparm::Parameters &params){
  return parameters_[0].Cross(params);
}

reparm::Header reparm::GaussianInput::GetHeader() const{
  return this->header_[0];
}

reparm::Coordinates reparm::GaussianInput::GetCoordinates() const{
  return this->coordinates_[0];
}

reparm::Parameters reparm::GaussianInput::GetParameters() const{
  return this->parameters_[0];
}

std::string reparm::GaussianInput::str() {
  std::string input = (this->header_[0].str() + this->coordinates_[0].str()
                       + this->parameters_[0].str());
  for (size_t i = 1; i < this->header_.size(); ++i){
    input += ("--Link1--\n" + this->header_[i].str() + this->coordinates_[i].str()
              + this->parameters_[i].str());
  }
  return input;
}

void reparm::GaussianInput::Link(const reparm::GaussianInput& rhs){
  this->header_.push_back(rhs.GetHeader());
  this->coordinates_.push_back(rhs.GetCoordinates());
  this->parameters_.push_back(rhs.GetParameters());
}
