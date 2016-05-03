#ifndef REPARM_INPUT_H
#define REPARM_INPUT_H

#include <string>
#include <reparm_exceptions.h>

class ReparmInput {
 private:
  std::string file_name_;
  std::string high_level_theory_;
  std::string molecule_name_;
  int number_excited_states_;
  int number_generations_;
  int number_processors_;
  int number_geometries_;
  int number_elites_;
  int population_size_;
  float mutation_rate_;
  float mutation_perturbation_;
  float survival_chance_;
  float geometric_perturbation_;
  bool should_continue_;

 public:
 ReparmInput(std::string file_name)
   : file_name_(file_name)
    , high_level_theory_("None")
    , molecule_name_("None")
    , number_excited_states_(0)
    , number_generations_(1)
    , number_processors_(1)
    , number_geometries_(1)
    , number_elites_(0)
    , population_size_(1)
    , mutation_rate_(0.1)
    , mutation_perturbation_(0.05)
    , survival_chance_(0.5)
    , geometric_perturbation_(0.05)
    , should_continue_(false)
    {ReadInputFile();}

  void SetHighLevelTheory(std::string);
  std::string GetHighLevelTheory() const;
  void SetMoleculeName(std::string);
  std::string GetMoleculeName() const;
  void SetNumberGenerations(int N);
  int GetNumberGenerations() const;
  void SetNumberProcessors(int N);
  int GetNumberProcessors() const;
  void SetNumberExcitedStates(int N);
  int GetNumberExcitedStates() const;
  void SetNumberGeometries(int N);
  int GetNumberGeometries() const;
  void SetNumberElites(int N);
  int GetNumberElites() const;
  void SetPopulationSize(int N);
  int GetPopulationSize() const;
  void SetMutationRate(float N);
  float GetMutationRate() const;
  void SetMutationPerturbation(float N);
  float GetMutationPerturbations() const;
  void SetSurvivalChance(float N);
  float GetSurvivalChance() const;
  void SetGeometricPerturbation(float N);
  float GetGeometricPerturbation() const;
  void SetShouldContinue(bool);
  bool GetShouldContinue() const;

  void ReadInputFile();

};

#endif /* REPARM_INPUT_H */
