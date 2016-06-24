#ifndef GIN_READER_H
#define GIN_READER_H

#include "header.h"
#include "coordinates.h"
#include "parameters.h"

namespace reparm{
    namespace gaussian{

        Header ReadHeader(const std::string &s);
        Coordinates ReadCoordinates(const std::string &s);
        Parameters ReadParameters(const std::string &s);

    }
}
#endif /* GIN_READER_H */
