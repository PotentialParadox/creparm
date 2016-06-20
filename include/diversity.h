#ifndef DIVERSITY_H
#define DIVERSITY_H

#include <memory>
#include <reparm_data.h>

namespace reparm {

    class Diversity {
    private:
        /***** OBJECTS *****/
        std::shared_ptr<ReparmData> reparm_data_;
        std::vector<ParameterGroup> sorted_set_;
        /* a copy of the original population, from
           which we will remove elements */
        std::vector<ParameterGroup> temp_set_;

        /***** Methods *****/
        void SelectNext();

        float DetermineValue(const ParameterGroup &);

    public:
        Diversity(std::shared_ptr<ReparmData> reparm_data)
                : reparm_data_{reparm_data} { }

        void Sort();

    };

}

#endif /* DIVERSITY_H */