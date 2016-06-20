#include <diversity.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <container_math.h>

namespace reparm{

  void Diversity::Sort(){
    /* We will be removing elements from this */
    temp_set_ = reparm_data_->population_;
    sorted_set_.clear();
    /* The best is the best, regardless of how differnt it is,
       so we first grab the best fitness. */
    auto it = std::min_element(temp_set_.begin(), temp_set_.end(), 
		     [] (const ParameterGroup &a, const ParameterGroup &b){
		       return a.GetFitness() > b.GetFitness();
		     });
    sorted_set_.push_back(*it);
    temp_set_.erase(std::remove(temp_set_.begin(), temp_set_.end(), *it), temp_set_.end());
  }

  void Diversity::SelectNext(){
  }

  float Diversity::DetermineValue(const ParameterGroup &param_group){
    float distance_sum = 0;
    pg_params = param_group.Ge
    for (const auto &i: sorted_set_){

    }
  }

}