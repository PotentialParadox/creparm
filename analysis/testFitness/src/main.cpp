#include <iostream>
#include <gaussian.h>
#include "../include/reparm_data.h"
#include "../include/fitness.h"

using namespace std;
using namespace reparm;

int main() {
    /* We load a reparm.in file containing the information
     * from our test set */
    ReparmData reparm_data{"reparm.in"};
    reparm_data.Load();

    /* We need to extract the parameter float values,
     * and override our population */
    GaussianInput gin{"test_parameters.com"};
    Parameters params = gin.GetParameters();
    reparm_data.population_[0].SetParameters(params);

    /* Create the output files */
    Gaussian gaussian(reparm_data.population_[0]);
    vector<GaussianOutput> gouts{gaussian.RunGaussian()};
    reparm_data.population_[0].SetOutputs(gouts);

    /* Find the fitness */
    Fitness fitness{reparm_data.population_, reparm_data.high_level_outputs_};
    fitness(reparm_data.population_[0]);
    cout << fitness.StringList(reparm_data.population_[0]);
    cout << "Total Fitness: " << reparm_data.population_[0].GetFitness() /
            reparm_data.GetOriginalFitness() << endl;
    return 0;
}