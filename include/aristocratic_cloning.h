#ifndef ARISTOCRATIC_CLONING_H
#define ARISTOCRATIC_CLONING_H

#include <vector>
#include <reparm_data.h>
#include <mutate.h>
#include <parameter_group.h>
#include <fitness.h>

namespace reparm{

class AristocraticCloning{

  private:
    int number_elites_;
    reparm::Mutate mutate_;

  public:
    AristocraticCloning(reparm::ReparmData&, const reparm::Fitness&);

    void operator () (std::vector<reparm::ParameterGroup>&);

};

}

#endif /* ARISTOCRATIC_CLONING_H */
