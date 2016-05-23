/* This parameter class is used to manage the parameters
 * found in a gaussian input
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 2016
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H     

#include <string>
#include <vector>

namespace reparm{

class Parameters{
  private:
    // Contains the string values for each group of nonspace values
    // in the input. Never updated after constructor is called.
    // The str() combines the lables and the floats.
    std::vector<std::string> parameter_labels_;

    // Stores the parameter float values, their corresponding label index
    // and their position within that value. Updated by mutate.
    std::vector<double> parameter_floats_;

  public:
    Parameters(){};
    Parameters(const std::string&);
    void ReadLabels(const std::string&);
    void ExtractFloats();
    void SetParameters(const std::vector<double>);
    reparm::Parameters Cross(const reparm::Parameters&);
    void Mutate(const double &pertubation, const float &mutation_rate);
    std::string str() const;
    bool empty() const;
};

}

#endif /* PARAMETERS_H */
