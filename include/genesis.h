#ifndef GENESIS_H
#define GENESIS_H

#include <memory>
#include <vector>
#include <reparm_data.h>
#include <coordinates.h>

namespace reparm{
  class Genesis{

  private:
    /**** Objects ****/
    std::shared_ptr<ReparmData> reparm_data_;
    std::vector<std::vector<float> > normal_modes_;
    std::vector<Coordinates> coordinates_;
    Coordinates opt_coord_;
    /* These parameters can either be given or 
       if not be calculated through an am1 run */
    Parameters init_parameters_;

    /**** Member Functions ****/
    /* Reads the gaussian input supplied by the user */
    void ReadUserInput();
    /* Finds the HLT optimized coordinates as well as 
       the normal modes */
    void FindHLTOptNormal();
    /* Uses the normal modes to create a vector of
       coordinate to be used for our sample */
    void CreateCoordinates();
    void CreatePopulation();
    void CreateHLT();
    

  public:
    Genesis(std::shared_ptr<ReparmData> &reparm_data);

  };

}
#endif /* GENESIS_H */
