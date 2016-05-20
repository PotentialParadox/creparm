#ifndef GENESIS_H
#define GENESIS_H

#include <memory>
#include <vector>
#include <reparm_data.h>
#include <coordinates.h>

namespace reparm{
  class Genesis{

  private:
    /* Objects */
    std::shared_ptr<ReparmData> reparm_data_;
    std::vector<std::vector<float> > normal_modes_;
    Coordinates opt_coord_;
    /* These parameters can either be given or 
       if not be calculated through an am1 run */
    Parameters init_parameters_;

    /* Member Functions */
    void ReadUserInput();
    void CreateHLTOptFreq();
    void RunHLTOptFreq();
    void FindOptFreq();
    
    

  public:
    Genesis(std::shared_ptr<ReparmData> &reparm_data);

  };

}
#endif /* GENESIS_H */