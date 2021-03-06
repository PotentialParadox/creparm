/* A functor that mutates the nonelites by default, but can be used to mutate
 * across an index. It then updates their corresponding fitness by running gaussian.
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 3/9/2016 */

#ifndef MUTATE_H
#define MUTATE_H

#include <reparm_data.h>
#include <parameter_group.h>
#include <fitness.h>
#include <gaussian.h>
#include <memory>

namespace reparm{

class Mutate{
  friend class AristocraticCloning;

private:
  std::shared_ptr<reparm::ReparmData> reparm_data_;
  Mutate(){};

public:
 Mutate(const std::shared_ptr<reparm::ReparmData> &reparm_data)
   : reparm_data_{reparm_data}
  {}

  void operator () (std::vector<reparm::ParameterGroup> &population);
  void operator () (std::vector<reparm::ParameterGroup> &population,
                    unsigned int begin, unsigned int end);

};

}

#endif /* MUTATE_H */
