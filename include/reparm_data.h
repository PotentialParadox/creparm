// This class acts as the primary struct to main and contains
// the highest level objects
// Created by Dustin Tracy (dtracy.uf@gmail.com) 3/7/2016
#ifndef REPARM_DATA_H
#define REPARM_DATA_H

#include <string>
#include <vector>
#include <reparm_input.h>
#include <parameter_group.h>
#include <gaussian_output.h>
#include <gaussian_input.h>

namespace reparm{

  class ReparmData{
    private:
      ReparmInput reparm_input_;
      std::vector<GaussianOutput> high_level_outputs_;

    public:
      ReparmData(std::string s)
        : reparm_input_{s}
        {}

      // population_ should be modifiable
      std::vector<ParameterGroup> population_;
      ReparmInput GetReparmInput();
      std::vector<reparm::ParameterGroup> CreatePopulation(GaussianInput &input);
      const std::vector<GaussianOutput>& GetHighLevelOutputs() const;
      void CalculateHighLevel();
  };

}
#endif /* REPARM_DATA_H */

