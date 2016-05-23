#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <string>
#include <reparm_exceptions.h>
#include <parameter_group.h>
#include <gaussian_output.h>

namespace reparm{

class Gaussian{
  private:
    // Use the full file, not the file name
    reparm::ParameterGroup param_group_;

  public:
    Gaussian()
      :param_group_{}
      {}
    Gaussian(const reparm::ParameterGroup &param_group)
      :param_group_{param_group}
      {}

    std::vector<reparm::GaussianOutput> RunGaussian();
    std::vector<reparm::GaussianOutput> RunGaussian(reparm::ParameterGroup&);
    std::string RunGaussian(std::string &input_file);

};

}

extern reparm::Gaussian g_run;

#endif /* GAUSSIAN_H */
