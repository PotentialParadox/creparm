/* A functor that mutates the nonelites by default, but can be used to mutate
 * across an index. It then updates their corresponding fitness by running gaussian.
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 3/9/2016 */

#ifndef MUTATE_H
#define MUTATE_H

#include <reparm_data.h>
#include <parameter_group.h>
#include <fitness.h>
#include <gaussian.h>

namespace reparm{

class Mutate{

private:
  double mutation_rate_;
  double mutation_perturbation_;
  int number_elites_;
  reparm::Fitness fitness_;
  Mutate();

public:
  Mutate(reparm::ReparmData reparm_data, reparm::Fitness fitness)
    : mutation_rate_{reparm_data.GetReparmInput().GetMutationRate()}
    , mutation_perturbation_{reparm_data.GetReparmInput().GetMutationPerturbations()}
    , number_elites_{reparm_data.GetReparmInput().GetNumberElites()}
    , fitness_{fitness}
  {}

  void operator () (std::vector<reparm::ParameterGroup> &population);
  void operator () (std::vector<reparm::ParameterGroup> &population,
                    unsigned int begin, unsigned int end);

};

}

#endif /* MUTATE_H */
