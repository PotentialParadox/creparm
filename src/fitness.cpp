#include <fitness.h>
#include <vector>
#include <iostream>
#include <parameter_group.h>
#include <cmath>
#include <sstream>

double Average(std::vector<double> &a);
double RMS(const std::vector<double> &a,const std::vector<double> &b);
double AverageRMS(const std::vector<std::vector<double> > &a,
                  const std::vector<std::vector<double> > &b);
double Distance(std::vector<double> s1, std::vector<double> s2);
double MergeDistance(const std::vector<double> &a, const std::vector<double> &b);
double SpecDistance(std::vector<std::vector<double> > &a,
                    std::vector<std::vector<double> > &b);
std::vector<double> Differences(const std::vector<double> v);
double AverageRMSDifferences(const std::vector<std::vector<double> > &a,
                             const std::vector<std::vector<double> > &b);
double EnergyFitness(const reparm::ParameterGroup &param_group,
                     const std::vector<reparm::GaussianOutput> &high_level_outputs);
double DipoleFitness(const reparm::ParameterGroup &param_group,
                     const std::vector<reparm::GaussianOutput> &high_level_outputs);
double ExcitedStateFitness(const reparm::ParameterGroup &param_group,
                           const std::vector<reparm::GaussianOutput> &high_level_outputs);
double IRSpecFitness(const reparm::ParameterGroup &param_group,
                     const std::vector<reparm::GaussianOutput> &high_level_outputs);
double Average3dDistance(std::vector<double> a, std::vector<double> b);
double AverageForceDistance(std::vector<std::vector<double> > a,
                            std::vector<std::vector<double> > b);
double ForceFitness(const reparm::ParameterGroup &param_group,
                    const std::vector<reparm::GaussianOutput> &high_level_outputs);

double Average(std::vector<double> &a){
  double value{0.0};
  for (auto &i: a){ value += i;}
  return value / static_cast<double>(a.size());
}

double RMS(const std::vector<double> &a,const std::vector<double> &b){
  if (a.size() != b.size())
    throw "RMS Failure: vector sizes are not the same";
  double variance = 0;
  for (size_t i = 0; i != a.size(); ++i)
    variance += pow(a[i] - b[i], 2);
  variance /= static_cast<double>(a.size());
  return sqrt(variance);
}

double AverageRMS(const std::vector<std::vector<double> > &a,
                  const std::vector<std::vector<double> > &b){
  if (a.size() != b.size())
    throw "RMS Failure: vector sizes are not the same";
  double avearge_rms = 0;
  for (size_t i = 0; i < a.size(); ++i)
    avearge_rms += RMS(a[i], b[i]);
  return avearge_rms / static_cast<double>(a.size());
}

double Distance(std::vector<double> s1, std::vector<double> s2){
  if (s1.size() != s2.size())
    throw "Distance Error: Vector Sizes are not the same";
  double sum = 0;
  for (size_t i = 0; i != s1.size(); ++i)
    sum += std::pow(s1[i] - s2[i], 2); 
  return std::sqrt(sum);
}

double MergeDistance(const std::vector<double> &a, const std::vector<double> &b){
  double distance = 0;
  for (size_t i = 0; i != a.size(); i += 2){
    std::vector<double> s1{a[i], a[i+1]}; 
    std::vector<double> s2{b[i], b[i+1]};
    distance += Distance(s1, s2);
  }
  return distance / static_cast<double>(a.size());
}

double SpecDistance(std::vector<std::vector<double> > &a,
                    std::vector<std::vector<double> > &b){
  if (a.size() != b.size())
    throw "Spectra Fit Error";
  double distance = 0;
  for (size_t i = 0; i != a.size(); ++i)
    distance += MergeDistance(a[i], b[i]);
  return distance / static_cast<double>(a.size());
}

double AverageRMSDifferences(const std::vector<std::vector<double> > &a,
                             const std::vector<std::vector<double> > &b){
  double average_rms = 0;
  for (size_t i = 0; i != a.size(); ++i){
    auto am1_differences = Differences(a[i]);
    auto hlt_differences = Differences(b[i]);
    average_rms += RMS(am1_differences, hlt_differences);
  }
  return average_rms / static_cast<double>(a.size());
}

std::vector<double> Differences(const std::vector<double> v){
  if (v.size() < 2)
    throw "Vector not large enough for finding differences";
  std::vector<double> differences;
  for (size_t i = 0; i < v.size() - 1; ++i)
    differences.push_back(v[i+1] - v[i]);
  return differences;
}

double EnergyFitness(const reparm::ParameterGroup &param_group,
                     const std::vector<reparm::GaussianOutput> &high_level_outputs){
  // Get AM1 Energies
  std::vector<double> am1_energies;
  for (auto &i: param_group.GetOutputs())
    am1_energies.push_back(i.GetEnergy());
  /* Convert to Differences */
  auto am1_differences = Differences(am1_energies);
  // Get HLT Energies
  std::vector<double> hlt_energies;
  for (auto &i: high_level_outputs)
    hlt_energies.push_back(i.GetEnergy());
  /* Convert to Differences */
  auto hlt_differences = Differences(hlt_energies);
  // Return the RMS
  return RMS(am1_differences, hlt_differences);
}

double DipoleFitness(const reparm::ParameterGroup &param_group,
                     const std::vector<reparm::GaussianOutput> &high_level_outputs){
  // Get AM1 Dipoles
  std::vector<std::vector<double> > am1_dipoles;
  for (const auto &i: param_group.GetOutputs())
    am1_dipoles.push_back(i.GetDipole());
  // Get HLT Dipoles
  std::vector<std::vector<double> > hlt_dipoles;
  for (const auto &i: high_level_outputs)
    hlt_dipoles.push_back(i.GetDipole());
  return AverageRMS(am1_dipoles, hlt_dipoles);
}

double ExcitedStateFitness(const reparm::ParameterGroup &param_group,
                           const std::vector<reparm::GaussianOutput> &high_level_outputs){
  // Get AM1 Excited States
  std::vector<std::vector<double> > am1_excited_states;
  for (const auto &i: param_group.GetOutputs())
    am1_excited_states.push_back(i.GetExcitedStates());
  // Get HLT Excited States
  std::vector<std::vector<double> > hlt_excited_states;
  for (const auto &i: high_level_outputs)
    hlt_excited_states.push_back(i.GetExcitedStates());
  return AverageRMSDifferences(am1_excited_states, hlt_excited_states);
}

double IRSpecFitness(const reparm::ParameterGroup &param_group,
                     const std::vector<reparm::GaussianOutput> &high_level_outputs){
  // Get AM1 IR Frequencies
  std::vector<std::vector<double> > am1_ir_frequencies;
  for (const auto &i: param_group.GetOutputs())
    am1_ir_frequencies.push_back(i.GetFrequencies());
  // Get DFT IR Frequencies
  std::vector<std::vector<double> > hlt_ir_frequencies;
  for (const auto &i: high_level_outputs)
    hlt_ir_frequencies.push_back(i.GetFrequencies());
  return AverageRMSDifferences(am1_ir_frequencies, hlt_ir_frequencies);
}

double Average3dDistance(std::vector<double> a, std::vector<double> b){
  if (a.size() != b.size())
    throw "3d Distance lengths differ";
  if (a.size() % 3){
    std::cout << "Size: ";
    std::cout << a.size() << std::endl;
    throw "Force vector not divisable by 3";
  }
  double distance = 0;
  for (size_t i = 0; i < a.size(); i += 3){
    std::vector<double> s1{a[i], a[i+1], a[i+2]};
    std::vector<double> s2{b[i], b[i+1], b[i+2]};
    distance += Distance(s1, s2);
  }
  return distance / static_cast<double>(a.size());
}

double AverageForceDistance(std::vector<std::vector<double> > a,
                            std::vector<std::vector<double> > b){
  if (a.size() != b.size())
    throw "Force vector sizes do not equate";
  double distance = 0;
  for (size_t i = 0; i != a.size(); ++i)
    distance += Average3dDistance(a[i], b[i]);
  return distance / static_cast<double>(a.size());
}

double ForceFitness(const reparm::ParameterGroup &param_group,
                    const std::vector<reparm::GaussianOutput> &high_level_outputs){
  // Get AM1 Forces
  std::vector<std::vector<double> > am1_forces;
  for (const auto &i: param_group.GetOutputs())
    am1_forces.push_back(i.GetForces());
  // Get HLT Forces
  std::vector<std::vector<double> > hlt_forces;
  for (const auto &i: high_level_outputs)
    hlt_forces.push_back(i.GetForces());
  return AverageForceDistance(am1_forces, hlt_forces);
}

reparm::Fitness::Fitness(const reparm::ParameterGroup &param_group,
                         const std::vector<reparm::GaussianOutput> &high_level_outputs)
  : high_level_outputs_{high_level_outputs}
{
  original_e_fitness_ = EnergyFitness(param_group, high_level_outputs_);
  original_d_fitness_ = DipoleFitness(param_group, high_level_outputs_);
  //original_es_fitness_ = ExcitedStateFitness(param_group, high_level_outputs_);
  //original_f_fitness_ = ForceFitness(param_group, high_level_outputs_);
  //original_ir_fitness_ = IRSpecFitness(param_group, high_level_outputs_);
} 

std::string reparm::Fitness::StringList(const reparm::ParameterGroup &param_group) const{
  std::stringstream ss;

  try{
    double e_fitness = (EnergyFitness(param_group, high_level_outputs_) / original_e_fitness_);
    ss << "Energy Fitness: ";
    ss << e_fitness << std::endl;

    double d_fitness = (DipoleFitness(param_group, high_level_outputs_) / original_d_fitness_);
    ss << "Dipole Fitness: ";
    ss << d_fitness << std::endl;

    //double es_fitness = (ExcitedStateFitness(param_group, high_level_outputs_) / 
                         //original_es_fitness_);
    //ss << "Excited State Fitness: ";
    //ss << es_fitness << std::endl;

    //double f_fitness = (ForceFitness(param_group, high_level_outputs_) / 
                         //original_f_fitness_);
    //ss << "Force Fitness: ";
    //ss << f_fitness << std::endl;

    //double ir_fitness = (IRSpecFitness(param_group, high_level_outputs_) / original_ir_fitness_);
    //ss << "IR Spec Fitness: ";
    //ss << ir_fitness << std::endl;

  }
  catch(const char* e){
    std::cout << "Ignoring Step" << std::endl;
  }

  return ss.str();
}

double reparm::Fitness::operator () (reparm::ParameterGroup &rhs) const{
  double fitness;
  double e_fitness = 0;
  double d_fitness = 0;
  double es_fitness = 0;
  double f_fitness = 0;
  double ir_fitness = 0;
  try{
    e_fitness = (EnergyFitness(rhs, high_level_outputs_) / original_e_fitness_);
    d_fitness = (DipoleFitness(rhs, high_level_outputs_) / original_d_fitness_);
    //es_fitness = (ExcitedStateFitness(rhs, high_level_outputs_) / original_es_fitness_);
    //f_fitness = (ForceFitness(rhs, high_level_outputs_) / original_f_fitness_);
    //ir_fitness = (IRSpecFitness(rhs, high_level_outputs_) / original_es_fitness_);
    double fit_sum = (
		     e_fitness
                     + d_fitness
                     //+ es_fitness
                     //+ f_fitness
                     //+ ir_fitness
                     );
    fitness = (
                (e_fitness / fit_sum) * e_fitness 
                + (d_fitness / fit_sum) * d_fitness 
                //+ (es_fitness / fit_sum) * es_fitness
                //+ (f_fitness / fit_sum) * f_fitness
                //+ (ir_fitness / fit_sum) * ir_fitness
              );
  }
  catch(const char* e){
    std::cout << e << std::endl;
    fitness = 10;
  }
  rhs.SetFitness(fitness);
  return fitness;
}
