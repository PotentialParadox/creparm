#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include "../include/reparm_input.h"

void reparm::ReparmInput::ReadInputFile(){
    std::string file;
    std::ifstream fin(this->file_name_);
    try{
        if (!fin) throw "Unable to open reparameter input";
        std::string line;
        while (getline(fin, line)){
            file += line + "\n";
        }

        std::regex p{"Input File:\\s+(.+.com)"};
        std::smatch m;
        if(std::regex_search (file, m,  p)){
            this->SetInputFile(m[1]);
        }
        else {throw "\"Input File:\" not found in input";}

        p = "High Level Theory:\\s+(.+)";
        if(std::regex_search (file, m,  p)){
            this->high_level_theory_ = m[1];
        }
        else {throw "\"High Level Theory:\" not found in input";}

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

        p = "Temperature:\\s+(.+)";
        if(std::regex_search (file, m,  p)){
            this->temperature_ = stoi(m[1]);
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

        p = "Continue From Last Run:\\s+(.+)";
        if(std::regex_search (file, m,  p)){
            if (m[1] == "true" || m[1] == "True")
                this->should_continue_ = true;
        }

    } catch(const char *e){
        std::cerr << "Exception occured: " << e << ".\n";
        throw "Reparm Input Error";
    }
}

void reparm::ReparmInput::SetHighLevelTheory(std::string s){
    this->high_level_theory_ = s;
}

std::string reparm::ReparmInput::GetHighLevelTheory() const{
    return this->high_level_theory_;
}

void reparm::ReparmInput::SetInputFile(std::string s){
    this->molecule_name_ = s;
}

std::string reparm::ReparmInput::GetInputFile() const{
    return this->molecule_name_;
}

void reparm::ReparmInput::SetNumberExcitedStates(int N){
    this->number_excited_states_ = N;
}

int reparm::ReparmInput::GetNumberExcitedStates() const{
    return this->number_excited_states_;
}

void reparm::ReparmInput::SetNumberGenerations(int N){
    this->number_generations_ = N;
}

int reparm::ReparmInput::GetNumberGenerations() const{
    return this->number_generations_;
}

void reparm::ReparmInput::SetNumberProcessors(int N){
    this->number_processors_ = N;
}

int reparm::ReparmInput::GetNumberProcessors() const{
    return this->number_processors_;
}

void reparm::ReparmInput::SetNumberGeometries(int N){
    this->number_geometries_ = N;
}

int reparm::ReparmInput::GetNumberGeometries() const{
    return this->number_geometries_;
}

void reparm::ReparmInput::SetNumberElites(int N){
    this->number_elites_ = N;
}

int reparm::ReparmInput::GetNumberElites() const{
    return this->number_elites_;
}

void reparm::ReparmInput::SetPopulationSize(int N){
    this->population_size_ = N;
}

void reparm::ReparmInput::SetTemperature(int N){
    this->temperature_ = N;
}

int reparm::ReparmInput::GetPopulationSize() const{
    return this->population_size_;
}

int reparm::ReparmInput::GetTemperature() const{
    return this->temperature_;
}

void reparm::ReparmInput::SetMutationRate(float N){
    this->mutation_rate_ = N;
}

float reparm::ReparmInput::GetMutationRate() const{
    return this->mutation_rate_;
}

void reparm::ReparmInput::SetMutationPerturbation(float N){
    this->mutation_perturbation_ = N;
}

float reparm::ReparmInput::GetMutationPerturbations() const{
    return this->mutation_perturbation_;
}

void reparm::ReparmInput::SetSurvivalChance(float N){
    this->survival_chance_ = N;
}

float reparm::ReparmInput::GetSurvivalChance() const{
    return this->survival_chance_;
}

void reparm::ReparmInput::SetGeometricPerturbation(float N){
    this->geometric_perturbation_ = N;
}

float reparm::ReparmInput::GetGeometricPerturbation() const{
    return this->geometric_perturbation_;
}

void reparm::ReparmInput::SetShouldContinue(bool should_continue){
    should_continue_ = should_continue;
}

bool reparm::ReparmInput::GetShouldContinue() const{
    return should_continue_;
}
