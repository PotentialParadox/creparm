/* Slightly mutated clones of the elites are created.
Since elites are notoriously narcissistic, the clones rebel.
The members of the pairs of host and clone that are most fit
survive.
Created by Dustin Tracy (dtracy.uf@gmail.com) April 11, 2016 */
#ifndef ARISTOCRATIC_CLONING_H
#define ARISTOCRATIC_CLONING_H

#include <vector>
#include <reparm_data.h>
#include <mutate.h>
#include <parameter_group.h>
#include <fitness.h>
#include <memory>

namespace reparm{

class AristocraticCloning{

  private:
    int number_elites_;
    reparm::Mutate mutate_;

  public:
    AristocraticCloning(std::unique_ptr<reparm::ReparmData>&);

    void operator () (std::vector<reparm::ParameterGroup>&, reparm::Fitness&);

};

}

#endif /* ARISTOCRATIC_CLONING_H */
