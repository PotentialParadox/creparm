//
// Created by Dustin Tracy on 6/24/16.
//
#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include <random>
#include <chrono>
#include <cmath>
#include <map>
#include "../include/coordinates.h"

reparm::Coordinates::Coordinates()
        : charge_(0)
        , multiplicity_(0)
{}

reparm::Coordinates::Coordinates(const std::string &s){
    ReadSpins(s);
    ReadCoordinates(s);
}

reparm::Coordinates::Coordinates
        (int charge, int multiplicity, std::vector<std::vector<float> > &coordinates)
        : charge_(charge)
        , multiplicity_(multiplicity)
        , coordinates_(coordinates)
{}

void reparm::Coordinates::ReadSpins(const std::string &s){
    std::regex p_spins{"(\\d+)\\s+(\\d+)\\s*\n"};
    std::smatch m;
    std::regex_search(s, m, p_spins);
    charge_ = stoi(m[1]);
    multiplicity_ = stoi(m[2]);
}

std::string Number(const std::string &s){
    std::map<std::string, std::string> periodic_table{{"H", "1"}, {"He", "2"}, {"Li", "3"},
                                                      {"Be", "4"}, {"B", "5"}, {"C", "6"},
                                                      {"N", "7"}, {"O", "8"}, {"F", "9"},
                                                      {"Ne", "10"}, {"Na", "11"}, {"Mg", "12"},
                                                      {"Al", "13"}, {"Si", "14"}, {"P", "15"},
                                                      {"S", "16"}, {"Cl", "17"}, {"Ar", "18"}};
    if (periodic_table.find(s) != periodic_table.end()){
        return periodic_table.find(s)->second;
    }
    return s;
}

void reparm::Coordinates::ReadCoordinates(const std::string &s){
    std::regex p_coord{"(\\S+)\\s+(-?\\d+\\.\\d+\\s+)(-?\\d+\\.\\d+\\s+)(-?\\d+\\.\\d+\\s*)"};
    std::sregex_iterator pos(s.cbegin(), s.cend(), p_coord);
    std::sregex_iterator end;
    for (; pos != end; ++pos){
        std::vector<float> coordinate;
        coordinate.push_back(stof(Number(pos->str(1))));
        coordinate.push_back(stof(pos->str(2)));
        coordinate.push_back(stof(pos->str(3)));
        coordinate.push_back(stof(pos->str(4)));
        this->coordinates_.push_back(coordinate);
    }
}

void reparm::Coordinates::Perturb(const float &p){
    for (auto &&i: this->coordinates_){
        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
        std::default_random_engine rand(seed);
        std::uniform_real_distribution<double> distribution(0.0, 1.0);
        float r = distribution(rand) * p;
        float theta = distribution(rand) * M_PI;
        float phi = distribution(rand) * 2 *  M_PI;
        i[1] += r * std::sin(theta) * std::sin(phi);
        i[2] += r * std::sin(theta) * std::cos(phi);
        i[3] += r * std::cos(theta);
    }

}

std::string reparm::Coordinates::str() const{
    std::stringstream coordinates;
    coordinates.setf(std::ios::fixed);
    coordinates << charge_ << " " << multiplicity_ << "\n";
    for (const auto &i: this->coordinates_){
        coordinates.precision(0);
        coordinates << i[0];
        coordinates.precision(6);
        if (i[1] < 0){
            coordinates << "       " << i[1];
        }
        else{
            coordinates << "        " << i[1];
        }
        for (int j = 2; j < 4; ++j){
            if (i[j] < 0){
                coordinates << "    " << i[j];
            }
            else{
                coordinates << "     " << i[j];
            }
        }
        coordinates << std::endl;
    }
    coordinates << std::endl;

    return coordinates.str();
}

void reparm::Coordinates::ClearCoordinates(){
    coordinates_.clear();
}

std::string reparm::Coordinates::XYZString() const{
    std::stringstream ss;
    ss << "\n";
    int number_atoms = coordinates_.size();
    ss << number_atoms << "\n";
    for (auto i: coordinates_){
        ss << i[0] << " " << i[1] << " ";
        ss << i[2] << " " << i[3] << "\n";
    }
    return ss.str();
}

