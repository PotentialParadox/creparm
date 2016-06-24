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
        int charge_;
        int multiplicity_;
        std::vector<std::vector<float> >  coordinates_;

    public:
        Coordinates();
        Coordinates(const std::string&);
        Coordinates(int charge, int multiplicity,
                    std::vector<std::vector<float> > &coordinates);
        /* These read functions only read gaussian input
           files */
        void ReadSpins(const std::string&);
        void ReadCoordinates(const std::string&);
        std::vector<std::vector<float> > GetCoordinates() const
        {return coordinates_;}
        int GetCharge() const {return charge_;}
        int GetMultiplicity() const {return multiplicity_;}
        void ClearCoordinates();
        void Perturb(const float&);
        std::string str() const;
        std::string XYZString() const;
    };

}

#endif /* COORDINATES_H */

