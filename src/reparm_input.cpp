#include <reparm_input.h>
#include <iostream>
#include <string>
#include <fstream>
#include <regex>

void ReparmInput::ReadInputFile(){
  std::string file;
  std::ifstream fin(this->file_name_);
  try{
    if (!fin) throw "Unable to open reparameter input"; 
    std::string line;
    while (getline(fin, line)){
      file += line + "\n";
    }

    std::regex p{"Molecule Name:\\s+(.+)"};
    std::smatch m;
    if(std::regex_search (file, m,  p)){
      this->SetMoleculeName(m[1]);
    }
    else {throw "Molecule name not found in input";}

    p = "High Level Theory:\\s+(.+)";
    if(std::regex_search (file, m,  p)){
      this->high_level_theory_ = m[1];
    }
    else {throw "high level theory not specified";}

    p = "Number of Excited States:\\s+(.+)";
    if(std::regex_search (file, m,  p)){
      this->number_excited_states_ = stoi(m[1]);
    }

    p = "Number of Generations:\\s+(.+)";
    if(std::regex_search (file, m,  p)){
      this->number_generations_ = stoi(m[1]);
    }

    p = "Number of Processors:\\s+(.+)";
    if(std::regex_search (file, m,  p)){
      this->number_processors_ = stoi(m[1]);
    }

    p = "Number of Geometries:\\s+(.+)";
    if(std::regex_search (file, m,  p)){
      this->number_geometries_ = stoi(m[1]);
    }

    p = "Number of Elites:\\s+(.+)";
    if(std::regex_search (file, m,  p)){
      this->number_elites_ = stoi(m[1]);
    }

    p = "Population Size:\\s+(.+)";
    if(std::regex_search (file, m,  p)){
      this->population_size_ = stoi(m[1]);
    }

    p = "Mutation Rate:\\s+(.+)";
    if(std::regex_search (file, m,  p)){
      this->mutation_rate_ = stof(m[1]);
    }

    p = "Mutation Perturbation:\\s+(.+)";
    if(std::regex_search (file, m,  p)){
      this->mutation_perturbation_ = stof(m[1]);
    }

    p = "Survival Chance:\\s+(.+)";
    if(std::regex_search (file, m,  p)){
      this->survival_chance_ = stof(m[1]);
    }

    p = "Geometric Perturbation:\\s+(.+)";
    if(std::regex_search (file, m,  p)){
      this->geometric_perturbation_ = stof(m[1]);
    }

  } catch(const char *e){
    std::cerr << "Exception occured: " << e << ".\n";
    throw "Reparm Input Error";
  }
}

void ReparmInput::SetHighLevelTheory(std::string s){
  this->high_level_theory_ = s;
}

std::string ReparmInput::GetHighLevelTheory() const{
  return this->high_level_theory_;
}

void ReparmInput::SetMoleculeName(std::string s){
  this->molecule_name_ = s;
}

std::string ReparmInput::GetMoleculeName() const{
  return this->molecule_name_;
}

void ReparmInput::SetNumberExcitedStates(int N){
  this->number_excited_states_ = N;
}

int ReparmInput::GetNumberExcitedStates() const{
  return this->number_excited_states_;
}

void ReparmInput::SetNumberGenerations(int N){
  this->number_generations_ = N;
}

int ReparmInput::GetNumberGenerations() const{
  return this->number_generations_;
}

void ReparmInput::SetNumberProcessors(int N){
  this->number_processors_ = N;
}

int ReparmInput::GetNumberProcessors() const{
  return this->number_processors_;
}

void ReparmInput::SetNumberGeometries(int N){
  this->number_geometries_ = N;
}

int ReparmInput::GetNumberGeometries() const{
  return this->number_geometries_;
}

void ReparmInput::SetNumberElites(int N){
  this->number_elites_ = N;
}

int ReparmInput::GetNumberElites() const{
  return this->number_elites_;
}

void ReparmInput::SetPopulationSize(int N){
  this->population_size_ = N;
}

int ReparmInput::GetPopulationSize() const{
  return this->population_size_;
}

void ReparmInput::SetMutationRate(float N){
  this->mutation_rate_ = N;
}

float ReparmInput::GetMutationRate() const{
  return this->mutation_rate_;
}

void ReparmInput::SetMutationPerturbation(float N){
  this->mutation_perturbation_ = N;
}

float ReparmInput::GetMutationPerturbations() const{
  return this->mutation_perturbation_;
}

void ReparmInput::SetSurvivalChance(float N){
  this->survival_chance_ = N;
}

float ReparmInput::GetSurvivalChance() const{
  return this->survival_chance_;
}

void ReparmInput::SetGeometricPerturbation(float N){
  this->geometric_perturbation_ = N;
}

float ReparmInput::GetGeometricPerturbation() const{
  return this->geometric_perturbation_;
}
