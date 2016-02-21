#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <string>
#include <reparm_exceptions.h>
#include <parameter_group.h>

namespace reparm{

class Gaussian{
  private:
    // Use the full file, not the file name
    reparm::ParameterGroup param_group_;

  public:
    Gaussian(const reparm::ParameterGroup &param_group)
      :param_group_{param_group}
      {}

      std::vector<std::string> RunGaussian();
      void ThreadRun(int i);

};

}

#endif /* GAUSSIAN_H */
