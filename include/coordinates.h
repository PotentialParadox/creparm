/* This coordinate class is used to manage the spin and coordinates
 * found in a gaussian input.
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 2016
 */

#ifndef COORDINATES_H
#define COORDINATES_H

#include <string>
#include <vector>

namespace reparm{

class Coordinates{
  private:
    std::string spins_;
    std::vector<std::vector<float> >  coordinates_; 

  public:
    Coordinates()
      : spins_{}
      , coordinates_{}
      {}
    Coordinates(const std::string&);
    void ReadSpins(const std::string&);
    void ReadCoordinates(const std::string&);
    void Perturb(const float&);
    std::string str() const;
};

}

#endif /* COORDINATES_H */
    
