/* The Breed class takes the current population now shrunken after their
retcent struggle of survival, and breeds them, exchanging parameter information
to create a new fully populated child population.
The elites are automatically made into children. */
#ifndef BREED_H
#define BREED_H

#include<vector>
#include<reparm_data.h>
#include<parameter_group.h>

namespace reparm{

class Breed{
  private:
    int population_size_;
    int number_elites_;
    Breed();

  public:
    Breed(reparm::ReparmData &reparm_data)
      : population_size_{reparm_data.GetReparmInput().GetPopulationSize()}
      , number_elites_{reparm_data.GetReparmInput().GetNumberElites()}
      {}

    void operator () (std::vector<reparm::ParameterGroup> &population);
};

}

#endif /* BREED_H */
