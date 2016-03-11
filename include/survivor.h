/* This functor will randomly kill off non-elite members of the population
 * using their fitness values as weight
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 3/10/2016 */
#ifndef SURVIVOR_H
#define SURVIVOR_H

#include <vector>
#include <reparm_data.h>
#include <parameter_group.h>

namespace reparm{

  class Survivor{
    private:
      int number_elites_;
      float survival_chance_;
      Survivor();

    public:
      Survivor(reparm::ReparmData reparm_data)
        : number_elites_{reparm_data.GetReparmInput().GetNumberElites()}
        , survival_chance_{reparm_data.GetReparmInput().GetSurvivalChance()}
        {}

      void operator () (std::vector<reparm::ParameterGroup>&);

  };

}

#endif /* SURVIVOR_H */
