#include <iostream>
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

    /* Find the fitness */
    Fitness fitness{reparm_data.population_, reparm_data.high_level_outputs_};
    cout << fitness.StringList(reparm_data.population_[0]);
    return 0;
}