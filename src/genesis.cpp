#include <genesis.h>
#include <gaussian.h>
#include <gin_reader.h>
#include <gout_reader.h>
#include <fstream>
#include <iostream>
#include <armadillo>

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
       equal to 3/2 kb T. That means the total energy
       of the molecule should be 3/2 kb T Na, where
       Na is the number of atoms. */
    int number_atoms = (normal_modes_[0].size() - 1) / 3;
    constexpr double kb = 1.380648E-23; // Boltzman const
    constexpr double T = 298;  // Temperature
    const double Et = 3/2 * kb * T * number_atoms;

    /* We now want to distribute this energy to each of
       the modes. */
    // arma::arma_rng::set_seed_random();
    int number_modes = normal_modes_.size();
    int number_geometries = reparm_data_->GetReparmInput().GetNumberGeometries();
    std::cout << opt_coord_.str() << std::endl;
    for (int geom = 0; geom != number_geometries; ++geom){
      arma::rowvec r_values(number_modes, arma::fill::randu);
      // r_values.print("r_values: ");
      double normalizer = std::accumulate(r_values.begin(),
					  r_values.end(),
					  0.0);
      r_values.for_each([normalizer](arma::rowvec::elem_type &val)
			{val /= normalizer;});
      arma::rowvec energy_per_mode = Et * r_values;
      // energy_per_mode.print("energies: ");

      /* Given the energy and force constants, we can find
	 the max displacements using hooks law, E = 1/2 kx^2 */
      arma::rowvec force_consts(normal_modes_.size());
      for (size_t i = 0; i != normal_modes_.size(); ++i)
	force_consts[i] = normal_modes_[i][0];
      // force_consts.print("force_consts: ");

      arma::rowvec max_displacements;
      max_displacements = 2 * energy_per_mode / force_consts;
      max_displacements.for_each( [](arma::rowvec::elem_type &val)
				  {val = std::sqrt(val) * 1E10;} );
      arma::mat m_max_displacements(number_modes, number_modes);
      /* A diagonal matrix will be more useful */
      m_max_displacements.zeros();
      for (size_t i = 0; i != max_displacements.size(); ++i)
	m_max_displacements(i,i) = max_displacements[i];
      // m_max_displacements.print("max_displ: ");

      /* We now get the coordinate displacements for each of the 
	 normal modes. To do this we create a matrix of the normalized
	 coordinate modes, where the rows represent each mode
	 and the columns represent the coordinate */
      arma::mat normal_displacement(number_modes, number_atoms * 3);
      for (size_t i = 0; i != normal_modes_.size(); ++i)
	for (size_t j = 1; j != normal_modes_[i].size(); ++j)
	  normal_displacement(i,j-1)= normal_modes_[i][j];
      arma::mat m_displacement = m_max_displacements * normal_displacement;
      // m_displacement.print("m_displacements: ");

      /* Sum the rows for each column to a vector representing the
	 displacement for each coordinant. */
      arma::rowvec displacements(number_atoms * 3);
      for (size_t i = 0; i != number_modes; ++i)
	for (size_t j = 0; j != number_atoms * 3; ++j)
	  displacements[j] += m_displacement(i, j);

      // displacements.print("displ: ");

      auto m_coordinates = opt_coord_.GetCoordinates();
      for (size_t i = 0; i != m_coordinates.size(); ++i)
	for (size_t j = 1; j != m_coordinates[0].size(); ++j) // Skip atom number
	  m_coordinates[i][j] += displacements[3*i + j - 1];

      Coordinates coordinates(opt_coord_.GetCharge(),
			      opt_coord_.GetMultiplicity(),
			      m_coordinates);

      std::cout << coordinates.str() << std::endl;
      coordinates_.push_back(coordinates);
    }
  }

}
