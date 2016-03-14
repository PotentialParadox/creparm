/* This functor is in charge of calculating the cost function in the genetic algorithm.
 * This is most likely the class that you will want to modify.
 * Note that this updates the fitness members found in the ParameterGroup class
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 3/8/2016 */
#ifndef FITNESS_H
#define FITNESS_H

#include <vector>
#include <parameter_group.h>

namespace reparm{

class Fitness{
  friend class Mutate;

  private:
    std::vector<double> original_fitness_;
    std::vector<reparm::GaussianOutput> high_level_outputs_;
    Fitness(){};

  public:
    Fitness(const ParameterGroup &population,
            const std::vector<reparm::GaussianOutput> &high_level_outputs);

    std::string StringList(const reparm::ParameterGroup &param_group) const;
    double operator () (reparm::ParameterGroup &rhs) const;
};

}

#endif /* FITNESS_H */
