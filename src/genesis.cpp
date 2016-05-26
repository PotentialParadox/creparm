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
    for (auto i: coordinates_)
      std::cout << i.str() << std::endl;
    CreatePopulation();
    std::cout << "Calculating HLT" << std::endl;
    CreateHLT();
    std::cout << "Finished with HLT" << std::endl;
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
    const int number_atoms = (normal_modes_[0].size() - 1) / 3;
    constexpr double kb = 1.380648E-23; // Boltzman const
    const double T = reparm_data_->GetReparmInput().GetTemperature();  // Temperature
    const double Et = 3/2 * kb * T * number_atoms;

    /* We now want to distribute this energy to each of
       the modes. */
    const int number_modes = normal_modes_.size();
    const int number_geometries = reparm_data_->GetReparmInput().GetNumberGeometries();
    coordinates_.push_back(opt_coord_);
    for (int geom = 1; geom != number_geometries; ++geom){
      arma::arma_rng::set_seed_random();
      arma::rowvec r_values(number_modes, arma::fill::randu);
      double normalizer = std::accumulate(r_values.begin(),
					  r_values.end(),
					  0.0);
      r_values.for_each([normalizer](arma::rowvec::elem_type &val)
			{val /= normalizer;});
      arma::rowvec energy_per_mode = Et * r_values;

      /* Given the energy and force constants, we can find
	 the max displacements using hooks law, E = 1/2 kx^2 */
      arma::rowvec force_consts(normal_modes_.size());
      for (size_t i = 0; i != normal_modes_.size(); ++i)
	force_consts[i] = normal_modes_[i][0];

      arma::rowvec max_displacements;
      max_displacements = 2 * energy_per_mode / force_consts;
      max_displacements.for_each( [](arma::rowvec::elem_type &val)
				  {val = std::sqrt(val) * 1E10;} );
      /* The modes can go in either direction, so we need to 
	 randomly asign a negative value to some of them */
      arma::rowvec r_direction(number_modes, arma::fill::randu);
      auto it_disp = max_displacements.begin();
      auto it_r = r_direction.begin();
      auto end = max_displacements.end();
      for (; it_disp != end; ++it_disp, ++it_r)
	if (*it_r < 0.5)
	  *it_disp = *it_disp * -1;
      // max_displacements.print("max_displ");
      /* A diagonal matrix will be more useful */
      arma::mat m_max_displacements(number_modes, number_modes);
      m_max_displacements.zeros();
      for (size_t i = 0; i != max_displacements.size(); ++i)
	m_max_displacements(i,i) = max_displacements[i];

      /* We now get the coordinate displacements for each of the 
	 normal modes. To do this we create a matrix of the normalized
	 coordinate modes, where the rows represent each mode
	 and the columns represent the coordinate */
      arma::mat normal_displacement(number_modes, number_atoms * 3);
      normal_displacement.zeros();
      for (size_t i = 0; i != normal_modes_.size(); ++i)
	for (size_t j = 1; j != normal_modes_[i].size(); ++j)
	  normal_displacement(i,j-1)= normal_modes_[i][j];
      arma::mat m_displacement = m_max_displacements * normal_displacement;

      /* Sum the rows for each column to a vector representing the
	 displacement for each coordinant. */
      arma::rowvec displacements(number_atoms * 3, arma::fill::zeros);
      for (size_t i = 0; i != number_modes; ++i)
	for (size_t j = 0; j != number_atoms * 3; ++j)
	  displacements[j] += m_displacement(i, j);


      auto m_coordinates = opt_coord_.GetCoordinates();
      for (size_t i = 0; i != m_coordinates.size(); ++i)
	for (size_t j = 1; j != m_coordinates[0].size(); ++j) // Skip atom number
	  m_coordinates[i][j] += displacements[3*i + j - 1];

      Coordinates coordinates(opt_coord_.GetCharge(),
			      opt_coord_.GetMultiplicity(),
			      m_coordinates);

      coordinates_.push_back(coordinates);
    }
  }
  
 
  void Genesis::CreatePopulation(){
    std::stringstream ss;
    ss << reparm_data_->GetReparmInput().GetNumberExcitedStates();
    std::string ne =  ss.str();
    reparm::Header first_header{"#P AM1(Input,Print) CIS(Singlets,NStates=" + ne + ") pop(full)\n\nAM1\n"};
    reparm::Header second_header{"#P AM1(Input, Print) freq\n\nAM1\n"};
    std::vector<reparm::GaussianInput> inputs;
    for (const auto i: coordinates_){
      GaussianInput gin;
      gin.SetHeader(first_header);
      gin.SetCoordinates(i);
      gin.SetParameters(init_parameters_);
      GaussianInput gin2 = gin;
      gin2.SetHeader(second_header);
      gin.Link(gin2);
      inputs.push_back(gin);
    }
    Gaussian gaussian(inputs);
    std::cout << "running gaussian" << std::endl;
    std::ofstream fout{"test.com"};
    fout << inputs[1].str() << std::endl;
    std::vector<reparm::GaussianOutput> gouts{gaussian.RunGaussian()};
    reparm::ParameterGroup param_group{inputs};
    param_group.SetOutputs(gouts);
    std::vector<reparm::ParameterGroup> population;
    int p = reparm_data_->GetReparmInput().GetPopulationSize();
    for (int i = 0; i < p; ++i)
      population.push_back(param_group);
    reparm_data_->population_ = population;
  }

  void Genesis::CreateHLT(){
    std::string hlt = reparm_data_->GetReparmInput().GetHighLevelTheory();
    std::stringstream ss;
    ss << reparm_data_->GetReparmInput().GetNumberExcitedStates();
    std::string ne{ss.str()};
    reparm::Parameters empty_params{""};
    reparm::Header first_header{"#P " + hlt + " CIS(Singlets,NStates=" + ne + ") pop(full)\n\nhi\n"};
    reparm::Header second_header{"#P " + hlt + " freq\n\nhi\n"};
    reparm::ParameterGroup am1_group{reparm_data_->population_[0]};
    std::vector<reparm::GaussianInput> am1_inputs = am1_group.GetInputs();
    std::vector<reparm::GaussianInput> hlt_inputs;
    for (auto &&i: am1_inputs){
      reparm::GaussianInput hlt_input;
      hlt_input.SetHeader(first_header);
      hlt_input.SetCoordinates(i.GetCoordinates());
      hlt_input.SetParameters(empty_params);
      reparm::GaussianInput freq{hlt_input};
      freq.SetHeader(second_header);
      hlt_input.Link(freq);
      hlt_inputs.push_back(hlt_input);
    }
    reparm::ParameterGroup hlgroup{hlt_inputs};
    reparm::Gaussian gaussian{hlgroup};
    try{
      reparm_data_->high_level_outputs_ = gaussian.RunGaussian();
    }
    catch(...){
      throw "Problem running high level calculations";
    }
  }

}
