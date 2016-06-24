/* This gaussian output class reads and stores the information
 * produced by gaussian 09.
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 2016
 */

#ifndef GAUSSIAN_OUTPUT_H
#define GAUSSIAN_OUTPUT_H

#include <vector>
#include <string>

namespace reparm{

    class GaussianOutput{
    private:
        double energy_;
        std::vector<double> dipole_;
        std::vector<double> es_frequencies_;
        std::vector<double> es_intensities_;
        std::vector<double> forces_;
        std::vector<double> frequencies_;
        std::vector<double> intensities_;

    public:
        GaussianOutput()
                : energy_{0}
        {}

        GaussianOutput(double energy,
                       std::vector<double> dipole,
                       std::vector<double> es_frequencies,
                       std::vector<double> es_intensities,
                       std::vector<double> forces,
                       std::vector<double> frequencies,
                       std::vector<double> intensities)
                : energy_{energy}
                , dipole_{dipole}
                , es_frequencies_{es_frequencies}
                , es_intensities_{es_intensities}
                , forces_{forces}
                , frequencies_{frequencies}
                , intensities_{intensities}
        {}

        GaussianOutput(const std::string&);
        double GetEnergy() const {return energy_;}
        std::vector<double> GetDipole() const {return dipole_;}
        std::vector<double> GetESFrequencies() const {return es_frequencies_;}
        std::vector<double> GetESIntensities() const {return es_intensities_;}
        std::vector<double> GetForces() const {return forces_;}
        std::vector<double> GetFrequencies() const {return frequencies_;}
        std::vector<double> GetIntensities() const {return intensities_;}
        std::string str() const;
    };

}

#endif /* GAUSSIAN_OUTPUT_H */
