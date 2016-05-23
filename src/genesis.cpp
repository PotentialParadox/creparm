#include <genesis.h>
#include <gaussian.h>
#include <gin_reader.h>
#include <gout_reader.h>
#include <fstream>
#include <iostream>

namespace reparm{

  Genesis::Genesis(std::shared_ptr<ReparmData> &reparm_data)
    :reparm_data_(reparm_data)
  {
    ReadUserInput();
    FindHLTOptNormal();
    CreateCoordinates();
  }

  void Genesis::ReadUserInput(){
    std::string in_file_name;
    in_file_name = reparm_data_->GetReparmInput().GetInputFile();
    GaussianInput input(in_file_name);
    opt_coord_ = input.GetCoordinates();

    /* We'll try to get the parameters from the user.
       If we can't, we make our own. */
    init_parameters_ = input.GetParameters();
    if (init_parameters_.empty()){
      std::string s_header{"#P AM1(Input, print)\n\nam1\n"};
      Header header{s_header};
      input.SetHeader(header);
      std::string s_input = input.str();
      Gaussian gaussian;
      std::string s_gout;
      s_gout = gaussian.RunGaussian(s_input);
      init_parameters_ = gaussian::FindParameters(s_gout);
    }
    if (init_parameters_.empty())
      throw "Could not find parameters";
  };

  void Genesis::FindHLTOptNormal(){
    /* We first need to create an gaussian input file */
    std::string s_header1;
    s_header1 = ("%chk=hlt\n#P "
		+ reparm_data_->GetReparmInput().GetHighLevelTheory()
		+ " opt\n\nhlt\n");
    Parameters parameters;
    GaussianInput gin1;
    gin1.SetHeader(s_header1);
    gin1.SetCoordinates(opt_coord_);
    gin1.SetParameters(parameters);

    /* We're going to link it to a freq calculation */
    GaussianInput gin2{gin1};
    std::string s_header2 = ("%chk=hlt\n#P "
    		 + reparm_data_->GetReparmInput().GetHighLevelTheory()
    		 + "freq geom=Checkpoint\n\nhlt\n");
    gin2.SetHeader(s_header2);
    gin2.ClearCoordinates();
    gin1.Link(gin2);
    
    /* We run the job */
    std::string s_gin{gin1.str()};
    std::string gout = g_run.RunGaussian(s_gin);
    std::ofstream fout{"test.log"};
    fout << gout;
    
    /* Now we update the coordinates and the normal modes */
    opt_coord_ = gaussian::FindOptCoordinates(gout);
    normal_modes_ = gaussian::FindNormalModes(gout);
  };
  
  void Genesis::CreateCoordinates(){
    /* Our goal is to make the average energy per atom
       equal to 3/2 kb T, so we should start by
       converting the normal modes into a vector
       of energies per mode. Gaussian normalizes
       the modes, so we only need the force
       constants to create these energies*/
    std::vector<float> normal_energies;
    int number_atoms = (normal_modes_[0].size() - 1) / 3;
    for (const auto &i: normal_modes_){
      float force_const = 100 * i[0];
      float energy_per_atom = 0;
      energy_per_atom = force_const * 0.5 / number_atoms;
      normal_energies.push_back(energy_per_atom);
    }

    /* Now make a vector of random numbers from 0 to 1 */

    /* Normalize this random number array such that the
       element wise product of random times energy, leads
       to our expected atom energy */
  }

}
