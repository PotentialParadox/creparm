#include <survivor.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

std::vector<double> Fitnesses(std::vector<reparm::ParameterGroup> &population){
  std::vector<double> fitnesses;
  auto it = population.begin();
  for (; it < population.end(); ++it)
    fitnesses.push_back(it->GetFitness());
  return fitnesses;
}

// Change this function if you want to adjust the weights.
// This is most likely the only thing you'll want to change.
std::vector<double> Fitness_to_Survival(std::vector<double> fitnesses){
  std::vector<double> chances;
  double e = 4;  // Change the exponential weight
  for (auto &i: fitnesses){
    chances.push_back(1/std::pow(i,e));
  }
  return chances;
}

struct Weights{
  int group_id_;
  double chance_;
  double lower_bound_;
  double upper_bound_;
  Weights(int group_id, double chance)
    : group_id_{group_id}
    , chance_{chance}
    , lower_bound_{0}
    , upper_bound_{0}
    {}
};

void Normalize(std::vector<Weights> &contestants){
  double normalizer = 0;
  for (const auto &i: contestants)
    normalizer += i.chance_;
  double percent = 0;
  for (auto &i: contestants){
    i.lower_bound_ = percent;
    percent += i.chance_ / normalizer;
    i.upper_bound_ = percent;
  }
}

int ChooseSurvivor(std::vector<Weights> &contestants){
  double seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine e(seed);
  std::uniform_real_distribution<double> dist(0, 1);
  double r = dist(e);
  auto it = contestants.begin();
  int chosen_one = 0;
  for (; it < contestants.end(); ++it){
    if (r >= it->lower_bound_ && r <= it->upper_bound_){
      chosen_one = it->group_id_;
      contestants.erase(it);
      break;
    }
  }
  return chosen_one;
}

std::vector<int> Voting(std::vector<Weights> &contestants, const double &survival_chance){
  std::vector<int> winners;
  int number_survivors = contestants.size() * survival_chance + 1;
  for (int i = 0; i < number_survivors; ++i){
    Normalize(contestants);
    winners.push_back(ChooseSurvivor(contestants));
  }
  return winners;
}

void reparm::Survivor::operator()(std::vector<reparm::ParameterGroup> &population){
  std::vector<double> fitnesses = Fitnesses(population);
  std::vector<double> chances = Fitness_to_Survival(fitnesses);
  std::vector<Weights> contestants;
  for (int i = number_elites_; i < static_cast<int>(chances.size()); ++i)
    contestants.push_back(Weights(i, chances[i]));
  std::vector<int> survivors;
  for (int i = 0; i < number_elites_; ++i)
    survivors.push_back(i);
  std::vector<int> players{Voting(contestants, survival_chance_)};
  survivors.insert(std::end(survivors), std::begin(players), std::end(players));
  std::sort(std::begin(survivors), std::end(survivors));
  std::vector<reparm::ParameterGroup> new_population;
  for (auto i: survivors){
    new_population.push_back(population[i]);
  }
  population = new_population;
}
