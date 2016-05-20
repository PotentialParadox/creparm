#include <iostream>
#include <aristocratic_cloning.h>

reparm::AristocraticCloning::AristocraticCloning
(std::shared_ptr<reparm::ReparmData> &reparm_data)
  : number_elites_{reparm_data->GetReparmInput().GetNumberElites()}
{
  // How much smaller we want the mutation rate to be
  // for the cloning process
  double division_factor = 5.0;
  double mutation_rate = reparm_data->GetReparmInput().GetMutationRate() / division_factor;
  double mutation_perturbation = (reparm_data->GetReparmInput().GetGeometricPerturbation() /
                                   division_factor);
  std::shared_ptr<reparm::ReparmData> reparm_mod(new reparm::ReparmData(*reparm_data));
  reparm_mod->GetReparmInput().SetMutationRate(mutation_rate);
  reparm_mod->GetReparmInput().SetMutationPerturbation(mutation_perturbation);
  reparm::Mutate mutate{reparm_mod};
  mutate_ = mutate;
}

void reparm::AristocraticCloning::operator()(std::vector<reparm::ParameterGroup> & population
					     , reparm::Fitness& fitness){
  auto clones = population;
  mutate_(clones, 0, number_elites_);
  fitness(clones);
  for (size_t i = 0; i < population.size(); ++i)
    if (!(population[i] < clones[i]))
      population[i] = clones[i];
}
