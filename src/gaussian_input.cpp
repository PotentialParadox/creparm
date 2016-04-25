#include <gaussian_input.h>
#include <regex>
#include <iostream>
#include <fstream>

reparm::GaussianInput::GaussianInput(const std::string &s)
  : header_(new std::vector<reparm::Header>)
  , coordinates_(new std::vector<reparm::Coordinates>)
  , parameters_(new std::vector<reparm::Parameters>)
{
  try{
  std::string file{ReadFile(s)};
  reparm::Header header{ReadHeader(file)};
  header_->push_back(header);
  reparm::Coordinates coordinates{ReadCoordinates(file)};
  coordinates_->push_back(coordinates);
  reparm::Parameters parameters{ReadParameters(file)};
  parameters_->push_back(parameters);
  }
  catch(...){
    std::cerr << "Gaussian Input Read Error" << std::endl;
    gaussian_input_error e;
    throw e;
  }
}

reparm::GaussianInput::GaussianInput(const reparm::GaussianInput& rhs){
  *header_ = *(rhs.header_);
  *coordinates_ = *(rhs.coordinates_);
  *parameters_ = *(rhs.parameters_);
}

reparm::GaussianInput& reparm::GaussianInput::operator=
(const reparm::GaussianInput& rhs){
  if (&rhs == this)
    return *this;
  *header_ = *(rhs.header_);
  *coordinates_ = *(rhs.coordinates_);
  *parameters_ = *(rhs.parameters_);
  return *this;
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
  regex p_header{"^(.+\n)+\n(.+\n)"};
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
  if (header_->empty())
    header_->push_back(header);
  for (auto &i: *header_) {i = header;}
}

void reparm::GaussianInput::SetCoordinates(const reparm::Coordinates &coordinates){
  if (coordinates_->empty())
    coordinates_->push_back(coordinates);
  for (auto &i: *coordinates_) {i = coordinates;}
}

void reparm::GaussianInput::SetParameters(const reparm::Parameters &parameters){
  if (parameters_->empty())
    parameters_->push_back(parameters);
  for (auto &i : *parameters_) {i = parameters;}
}

void reparm::GaussianInput::PerturbCoordinates(const float &p){
  (*coordinates_)[0].Perturb(p);
  for (auto &i: *coordinates_) {i = (*coordinates_)[0];}
}

void reparm::GaussianInput::MutateParameters(const double &p, const float &r){
  (*parameters_)[0].Mutate(p, r);
  for (auto &i: *parameters_) {i = (*parameters_)[0];}
}

reparm::Parameters reparm::GaussianInput::Cross(const reparm::Parameters &params){
  return (*parameters_)[0].Cross(params);
  for (auto &i: *parameters_) {i = (*parameters_)[0];}
}

reparm::Header reparm::GaussianInput::GetHeader() const{
  return (*header_)[0];
}

reparm::Coordinates reparm::GaussianInput::GetCoordinates() const{
  return (*coordinates_)[0];
}

reparm::Parameters reparm::GaussianInput::GetParameters() const{
  return (*parameters_)[0];
}

std::string reparm::GaussianInput::str() {
  std::string input = ((*header_)[0].str() + (*coordinates_)[0].str()
                       + (*parameters_)[0].str());
  for (size_t i = 1; i < this->header_->size(); ++i){
    input += ("--Link1--\n" + (*header_)[i].str() + (*coordinates_)[i].str()
              + (*parameters_)[i].str());
  }
  return input;
}

void reparm::GaussianInput::ClearCoordinates(){
  for (auto &i: *coordinates_){ i.ClearCoordinates(); }
}

void reparm::GaussianInput::Link(const reparm::GaussianInput& rhs){
  this->header_->push_back(rhs.GetHeader());
  this->coordinates_->push_back(rhs.GetCoordinates());
  this->parameters_->push_back(rhs.GetParameters());
}
