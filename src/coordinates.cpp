#include <coordinates.h>
#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include <random>
#include <chrono>
#include <cmath>

reparm::Coordinates::Coordinates(const std::string &s){
  ReadSpins(s);
  ReadCoordinates(s);
}

void reparm::Coordinates::ReadSpins(const std::string &s){
  std::regex p_spins{"\\d+\\s+\\d+\\s*\n"};
  std::smatch m;
  std::regex_search(s, m, p_spins);
  this->spins_ = m[0];
}

void reparm::Coordinates::ReadCoordinates(const std::string &s){
  std::regex p_coord{"(\\d+)\\s+(-?\\d+\\.\\d+\\s+)(-?\\d+\\.\\d+\\s+)(-?\\d+\\.\\d+\\s+)"};
  std::sregex_iterator pos(s.cbegin(), s.cend(), p_coord);
  std::sregex_iterator end;
  for (; pos != end; ++pos){
    std::vector<float> coordinate;
    coordinate.push_back(stof(pos->str(1)));
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
  coordinates << this->spins_;
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
