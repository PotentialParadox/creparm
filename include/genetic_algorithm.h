#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <string>
#include <vector>
#include <reparm_input.h>
#include <reparm_data.h>
#include <parameter_group.h>
#include <gaussian_output.h>

namespace reparm{

  reparm::GaussianInput CreateReparmGaussian
    (std::string, reparm::ReparmInput &reparm_input);
  std::string PrintTitle();

}
#endif /* GENETIC_ALGORITHM_H */

