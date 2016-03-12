#include <breed.h>
#include <chrono>
#include <iostream>
#include <random>

void reparm::Breed::operator()(std::vector<reparm::ParameterGroup> &population){
  std::vector<reparm::ParameterGroup> child_population;
  int number_parents{static_cast<int>(population.size())};
  for (int i = 0; i < number_elites_; ++i)
    child_population.push_back(population[i]);
  double seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine e(seed);
  std::uniform_int_distribution<int> dist(0, number_parents - 1);
  for (int i = number_elites_; i < population_size_; ++i){
    int parent1 = dist(e);
    int parent2 = dist(e);
    while (parent1 == parent2){
      parent2 = dist(e);
    }
    reparm::ParameterGroup child = population[parent1].Cross(population[parent2]);
    child_population.push_back(child);
  }
  population = child_population;
}
