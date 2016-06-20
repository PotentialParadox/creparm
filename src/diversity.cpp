#include <diversity.h>
#include <iostream>
#include <container_math.h>

namespace reparm {

    void Diversity::Sort() {
        /* We will be removing elements from this */
        temp_set_ = reparm_data_->population_;
        sorted_set_.clear();
        /* The best is the best, regardless of how differnt it is,
           so we first grab the best fitness. */
        auto it = std::min_element(temp_set_.begin(), temp_set_.end(),
                                   [](const ParameterGroup &a, const ParameterGroup &b) {
                                       return a.GetFitness() > b.GetFitness();
                                   });
        sorted_set_.push_back(*it);
        temp_set_.erase(std::remove(temp_set_.begin(), temp_set_.end(), *it), temp_set_.end());
        while (sorted_set_.size() < reparm_data_->population_.size()) {
            SelectNext();
        }
    }

    void Diversity::SelectNext() {
        auto it = std::min_element(temp_set_.begin(), temp_set_.end(), SortFunction);
        sorted_set_.push_back(*it);
        temp_set_.erase(std::remove(temp_set_.begin(), temp_set_.end(), *it), temp_set_.end());
    }

    float Diversity::DetermineValue(const ParameterGroup &param_group) {
        float distance_sum = 0;
        auto pg_params = param_group.GetParameters().GetParameters();
        for (const auto &i: sorted_set_) {
            auto i_params = i.GetParameters().GetParameters();
            distance_sum += dmath::Distance(i_params.begin(), i_params.end(), pg_params.begin());
        }
        return distance_sum / sorted_set_.size();
    }

    bool Diversity::SortFunction(const ParameterGroup &a,
                                 const ParameterGroup &b) {
        return DetermineValue(a) < DetermineValue(b);
    }

}