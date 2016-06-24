#include <regex>
#include "../include/gin_reader.h"

namespace reparm{
    namespace gaussian{

        Header ReadHeader(const std::string &s){
            using namespace std;
            regex p_header{"^(.+\n)+\n(.+\n)"};
            smatch m;
            if (regex_search(s, m, p_header))
                return Header{m[0]};
            else
                throw "could not read header";
            return Header();
        }

        Coordinates ReadCoordinates(const std::string &s){
            using namespace std;
            regex p_coordinates{"\n\\s*\\d+\\s+\\d+\\s*\n(\\s*\\S+\\s+-?\\d+\\..+)+"};
            smatch m;
            if (regex_search(s, m, p_coordinates))
                return Coordinates{m[0]};
            else
                throw "could not read coordinates";
            return Coordinates();
        }

        Parameters ReadParameters(const std::string &s){
            using namespace std;
            regex p_parameters{"Method=(.|\n)*?(?=\\-\\-Link1)"};
            smatch m;
            if (regex_search(s, m, p_parameters))
                return Parameters{m[0]};
            else
                throw "could not read parameters";
            return Parameters();

        }

    }
}
