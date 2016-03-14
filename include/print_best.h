#ifndef PRINT_BEST_H
#define PRINT_BEST_H

#include <vector>
#include <reparm_data.h>
#include <parameter_group.h>
#include <gaussian_input.h>

namespace reparm{

  class PrintBest{
    private:
      reparm::ReparmData &reparm_data_;
      int number_excited_states_;

    public:
      PrintBest(reparm::ReparmData &reparm_data);

      void operator () ();
  };

}

#endif /* PRINT_BEST_H */
