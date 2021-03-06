/* The Breed class takes the current population now shrunken after their
retcent struggle of survival, and breeds them, exchanging parameter information
to create a new fully populated child population.
The elites are automatically made into children. 
Created by Dustin Tracy (dtracy.ufl.edu) April 11 2016*/
#ifndef BREED_H
#define BREED_H

#include<vector>
#include<reparm_data.h>
#include<parameter_group.h>
#include<memory>

namespace reparm{

  class Breed{
  private:
    std::shared_ptr<reparm::ReparmData> reparm_data_;
    Breed();

  public:
  Breed(const std::shared_ptr<reparm::ReparmData> &reparm_data)
    : reparm_data_{reparm_data}
    {}

    void operator () (std::vector<reparm::ParameterGroup> &population);
};

}

#endif /* BREED_H */
