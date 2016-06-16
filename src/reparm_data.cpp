#include <reparm_data.h>
#include <system_tools.hpp>
#include <gaussian_input.h>
#include <header.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <gaussian.h>
#include <fstream>
#include <container_math.h>

reparm::ReparmInput reparm::ReparmData::GetReparmInput() const{
  return reparm_input_;
}

const std::vector<reparm::GaussianOutput>& reparm::ReparmData::GetHighLevelOutputs() const{
  return high_level_outputs_;
}

void reparm::ReparmData::RunBest(){
  reparm::GaussianInput input_freq{population_[0].GetInputs()[0]};
  reparm::GaussianInput input_es{population_[0].GetInputs()[0]};
  reparm::Header header1{"\%chk=best\n#P AM1(Input,Print) opt\n\nbest\n"};
  input_freq.SetHeader(header1);
  
  /* Create an input file for caculating frequencies */
  reparm::GaussianInput input2{input_freq};
  reparm::Header header2{"\%chk=best\n#P AM1(Input,Print) freq geom=Checkpoint\n\nbest1\n"};
  input2.SetHeader(header2);
  input2.ClearCoordinates();
  input_freq.Link(input2);
  std::ofstream fout{"best_freq.com"};
  fout << input_freq.str();

  /* Create an input file for caculating excited_states */
  reparm::GaussianInput input3{input_freq};
  reparm::Header header3{"\%chk=best\n#P AM1(Input,Print) CIS(Singlets,NStates=" +
                         std::to_string(GetReparmInput().GetNumberExcitedStates()) +
                         ") pop(full) geom=Checkpoint\n\nbest1\n"};
  input3.SetHeader(header3);
  input3.ClearCoordinates();
  input_freq.Link(input3);
  std::ofstream fout_es{"best_es.com"};
  fout_es << input_freq.str();

  
  fout.close();
  fout_es.close();
  std::string command{"g09 best_freq.com best_freq.log"};
  systls::exec(command, 100000);
  command = "g09 best_es.com best_es.log";
  systls::exec(command, 100000);
}

void reparm::ReparmData::Save() const{
  std::ofstream fout{"reparm.dat"};
  /* For readability I initialize some aliases */
  int number_geometries = GetReparmInput().GetNumberGeometries();
  int number_atoms = population_[0].GetInputs()[0].GetCoordinates().GetCoordinates().size();
  int charge = population_[0].GetInputs()[0].GetCoordinates().GetCharge();
  int multiplicity = population_[0].GetInputs()[0].GetCoordinates().GetMultiplicity();
  /* I'm going to save all the coordinates, including the atom number to one vector */
  auto inputs = population_[0].GetInputs();

  std::vector<double> best_parameter_floats;
  best_parameter_floats = inputs[0].GetParameters().GetParameters();

  std::vector<std::string> parameter_labels;
  parameter_labels = inputs[0].GetParameters().GetLabels();

  std::vector<float> coordinates;
  for (const auto &i: inputs){
    auto coord_array = i.GetCoordinates().GetCoordinates();
    for (const auto j: coord_array)
      coordinates.insert(std::end(coordinates), std::begin(j), std::end(j));
  }
  std::vector<float> energies;
  for (const auto &i: GetHighLevelOutputs())
    energies.push_back(i.GetEnergy());
  std::vector<float> dipoles;
  for (const auto &i: GetHighLevelOutputs()){
    for (const auto &j: i.GetDipole())
      dipoles.push_back(j);
  }
  std::vector<float> es_frequencies;
  for (const auto &i: GetHighLevelOutputs()){
    for (const auto &j: i.GetESFrequencies())
      es_frequencies.push_back(j);
  }
  std::vector<float> es_intensities;
  for (const auto &i: GetHighLevelOutputs()){
    for (const auto &j: i.GetESIntensities())
      es_intensities.push_back(j);
  }
  std::vector<float> forces;
  for (const auto &i: GetHighLevelOutputs()){
    for (const auto &j: i.GetForces())
      forces.push_back(j);
  }
  std::vector<float> ir_frequencies;
  for (const auto &i: GetHighLevelOutputs()){
    for (const auto &j: i.GetFrequencies())
      ir_frequencies.push_back(j);
  }
  std::vector<float> ir_intensities;
  for (const auto &i: GetHighLevelOutputs()){
    for (const auto &j: i.GetIntensities())
      ir_intensities.push_back(j);
  }
    
  fout << number_geometries << std::endl;
  fout << number_atoms << std::endl;
  fout << charge << std::endl;
  fout << multiplicity << std::endl;
  fout << best_parameter_floats.size() << std::endl;
  for (auto i: best_parameter_floats)
    fout << i << " ";
  fout << std::endl;
  fout << parameter_labels.size() << std::endl;
  for (auto i: parameter_labels){
    if (i != "\n")
      fout << i << std::endl; 
    else
      fout << "\\n" << std::endl;
  }
  fout << coordinates.size() << std::endl;
  for (auto i: coordinates)
    fout << i << " ";
  fout << std::endl;
  fout << energies.size() << std::endl;
  for (auto i: energies)
    fout << i << " ";
  fout << std::endl;
  fout << dipoles.size() << std::endl;
  for (auto i: dipoles)
    fout << i << " ";
  fout << std::endl;
  fout << es_frequencies.size() << std::endl;
  for (auto i: es_frequencies)
    fout << i << " ";
  fout << std::endl;
  fout << es_intensities.size() << std::endl;
  for (auto i: es_intensities)
    fout << i << " ";
  fout << std::endl;
  fout << forces.size() << std::endl;
  for (auto i: forces)
    fout << i << " ";
  fout << std::endl;
  fout << ir_frequencies.size() << std::endl;
  for (auto i: ir_frequencies)
    fout << i << " ";
  fout << std::endl;
  fout << ir_intensities.size() << std::endl;
  for (auto i: ir_intensities)
    fout << i << " ";
  fout << std::endl;
}

void reparm::ReparmData::Load(){
  std::ifstream fin{"reparm.dat"};

  fin >> number_geometries_ >> number_atoms_ >> charge_ >> multiplicity_;
  int vector_size;
  fin >> vector_size;
  for (int i = 0; i != vector_size; ++i){
    double val = 0;
    fin >> val;
    best_parameters_.push_back(val);
  }
  std::string s_vec_size;
  std::getline(fin, s_vec_size);
  std::getline(fin, s_vec_size);
  vector_size = std::stoi(s_vec_size);
  for (int i = 0; i != vector_size; ++i){
    std::string val;
    std::getline(fin, val);
    if (val == "\\n"){
      val = "\n";
      parameter_labels_.push_back(val);
    }
    else if (val == ""){
      parameter_labels_[i-1] += "\n";
      --i;
    }
    else
      parameter_labels_.push_back(val);
  }
  fin >> vector_size;
  for (int i = 0; i != vector_size; ++i){
    float val = 0;
    fin >> val;
    coordinates_.push_back(val);
  }
  fin >> vector_size;
  for (int i = 0; i != vector_size; ++i){
    float val = 0;
    fin >> val;
    energies_.push_back(val);
  }
  fin >> vector_size;
  for (int i = 0; i != vector_size; ++i){
    float val = 0;
    fin >> val;
    dipoles_.push_back(val);
  }
  fin >> vector_size;
  for (int i = 0; i != vector_size; ++i){
    float val = 0;
    fin >> val;
    es_frequencies_.push_back(val);
  }
  fin >> vector_size;
  for (int i = 0; i != vector_size; ++i){
    float val = 0;
    fin >> val;
    es_intensities_.push_back(val);
  }
  fin >> vector_size;
  for (int i = 0; i != vector_size; ++i){
    float val = 0;
    fin >> val;
    forces_.push_back(val);
  }
  fin >> vector_size;
  for (int i = 0; i != vector_size; ++i){
    float val = 0;
    fin >> val;
    ir_frequencies_.push_back(val);
  }
  fin >> vector_size;
  for (int i = 0; i != vector_size; ++i){
    float val = 0;
    fin >> val;
    ir_intensities_.push_back(val);
  }
  CheckInput();
  LoadPopulation();
  LoadHLT();
}

void reparm::ReparmData::LoadPopulation(){
    std::stringstream ss;
    ss << GetReparmInput().GetNumberExcitedStates();
    std::string ne =  ss.str();
    std::vector<reparm::Coordinates> geometries = LoadGeometries();
    reparm::Parameters best_parameters;
    best_parameters.SetLabels(parameter_labels_);
    best_parameters.SetParameters(best_parameters_);
    reparm::Header first_header{"#P AM1(Input,Print) CIS(Singlets,NStates=" + ne + ") pop(full)\n\nAM1\n"};
    reparm::Header second_header{"#P AM1(Input, Print) freq\n\nAM1\n"};
    std::vector<reparm::GaussianInput> inputs;
    for (int i = 0; i != number_geometries_; ++i){
      GaussianInput gin;
      gin.SetHeader(first_header);
      gin.SetCoordinates(geometries[i]);
      gin.SetParameters(best_parameters);
      auto gin2 = gin;
      gin2.SetHeader(second_header);
      gin.Link(gin2);
      inputs.push_back(gin);
    }
    reparm::GaussianOutput gout = g_run.RunGaussian(inputs[0]);
    std::vector<reparm::GaussianOutput> gouts(number_geometries_, gout);
    int population_size = GetReparmInput().GetPopulationSize();
    for (int i = 0; i != population_size; ++i){
      reparm::ParameterGroup param_group{inputs};
      param_group.SetOutputs(gouts);
      population_.push_back(param_group);
    }
}

std::vector<reparm::Coordinates> reparm::ReparmData::LoadGeometries(){
  std::vector<reparm::Coordinates> geometries;
  int elems_geom = coordinates_.size() / number_geometries_;
  for (size_t i = 0; i != coordinates_.size(); i += elems_geom){
    std::vector<std::vector<float> > coordinates;
    for (size_t j = i; j != i + elems_geom; j += 4){
      std::vector<float> v = {coordinates_[j],
			      coordinates_[j+1],
			      coordinates_[j+2],
			      coordinates_[j+3]};
      coordinates.push_back(v);
    }
    reparm::Coordinates geometry{charge_, multiplicity_, coordinates};
    geometries.push_back(geometry);
  }
  return geometries;
}

bool reparm::ReparmData::CheckInput(){
  int user_number_geometries = GetReparmInput().GetNumberGeometries();
  if (number_geometries_ != user_number_geometries)
    throw "Loaded data contains a different number of geometries than the user input";
  return true;
}

void reparm::ReparmData::LoadHLT(){
  int n_energies_geom = 1;
  int n_dipoles_geom = 3;
  int n_es_freq_geom = es_frequencies_.size() / number_geometries_;
  int n_es_int_geom = es_intensities_.size() / number_geometries_;
  int n_forces_geom = forces_.size() / number_geometries_;
  int n_ir_freq_geom = ir_frequencies_.size() / number_geometries_;
  int n_ir_int_geom = ir_intensities_.size() / number_geometries_;

  /* Convert our vectors into a mutidimensional vector, where the outside
     vector represents the geometry */
  auto energies_geom = dmath::VectorOneToTwo(energies_, n_energies_geom);
  auto dipoles_geom = dmath::VectorOneToTwo(dipoles_, n_dipoles_geom);
  auto es_freq_geom = dmath::VectorOneToTwo(es_frequencies_, n_es_freq_geom);
  auto es_int_geom = dmath::VectorOneToTwo(es_intensities_, n_es_int_geom);
  auto forces_geom = dmath::VectorOneToTwo(forces_, n_forces_geom);
  auto ir_freq_geom = dmath::VectorOneToTwo(ir_frequencies_, n_ir_freq_geom);
  auto ir_int_geom = dmath::VectorOneToTwo(ir_intensities_, n_ir_int_geom);


  for (size_t i = 0; i != number_geometries_; ++i){
    auto energy = energies_geom[i][0];
    auto dipoles = dipoles_geom[i];
    auto es_frequencies = es_freq_geom[i];
    auto es_intensities = es_int_geom[i];
    auto forces = forces_geom[i];
    auto ir_frequencies = ir_freq_geom[i];
    auto ir_intensities = ir_int_geom[i];
    reparm::GaussianOutput hlt_out(energy,
				   dipoles,
				   es_frequencies,
				   es_intensities,
				   forces,
				   ir_frequencies,
				   ir_intensities);
    high_level_outputs_.push_back(hlt_out);
  }
}

bool reparm::ReparmData::Adjust(int steps_since_last_best){
  return true;
};



