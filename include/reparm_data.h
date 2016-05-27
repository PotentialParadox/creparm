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
    /****** OBJECTS *****/
    ReparmInput reparm_input_;
    int number_geometries_;
    int number_atoms_;
    int charge_;
    int multiplicity_;
    double original_fitness_;
    std::vector<float> coordinates_;
    std::vector<double> best_parameters_;
    std::vector<std::string> parameter_labels_;

    /****** METHODS *****/
    void LoadHLT();
    void LoadPopulation();
    std::vector<reparm::Coordinates> LoadGeometries();
    bool CheckInput();

  public:
    ReparmData(std::string s)
      : reparm_input_{s}
      , number_geometries_{0}
      , number_atoms_{0}
      , charge_{0}
      , multiplicity_{0}
      , original_fitness_{0}
    {}

    void Save() const;
    void Load();
    std::vector<GaussianOutput> high_level_outputs_;
    // population_ should be modifiable
    std::vector<ParameterGroup> population_;
    ReparmInput GetReparmInput() const;
    const std::vector<GaussianOutput>& GetHighLevelOutputs() const;
    void RunBest();
  };

}
#endif /* REPARM_DATA_H */

