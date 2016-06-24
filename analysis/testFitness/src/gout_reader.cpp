#include <string>
#include "../include/gout_reader.h"

namespace reparm{
    namespace gaussian{

        double FindEnergy(const std::string &s){
            std::regex p_energy{"\n\\s*SCF\\s+Done.*"};
            std::smatch m;
            if(!std::regex_search(s, m, p_energy)){
                std::cerr << "Cannot find energies" << std::endl;
                throw "Error";
            }
            std::string e_line{m[0].str()};
            std::regex p_float{"-?\\d+\\.\\w+-?\\w*"};
            std::regex_search(e_line, m, p_float);
            return stod(m[0].str());
        }

        std::vector<double> FindDipole(const std::string &s){
            std::regex p_dipole{"Charge=.*\n.*Dipole\\s+moment.*\n.*"};
            std::sregex_iterator pos(s.begin(), s.end(), p_dipole);
            std::sregex_iterator end;
            std::vector<std::string> dipole_list;
            for (; pos != end; ++pos)
                dipole_list.push_back(pos->str(0));
            std::string last_dipole{*(dipole_list.end() - 1)};

            std::regex p_x{"X=\\s+(-?\\d+\\.\\d+)\\s+"};
            std::smatch m;
            std::regex_search(last_dipole, m, p_x);
            double x{stod(m[1])};

            std::regex p_y{"Y=\\s+(-?\\d+\\.\\d+)\\s+"};
            std::regex_search(last_dipole, m, p_y);
            double y{stod(m[1])};

            std::regex p_z{"Z=\\s+(-?\\d+\\.\\d+)\\s+"};
            std::regex_search(last_dipole, m, p_z);
            double z{stod(m[1])};

            return std::vector<double>{x, y, z};
        }

        std::vector<double> FindForces(const std::string &s){
            std::regex p_forces{"Forces\\s+\\(.*\n.*\n.*\n(\\s+\\d+\\s+\\d+\\s+.*\n)*"};
            std::smatch m;
            std::regex_search(s, m, p_forces);
            std::string force_string = m[0];
            std::regex p_floats{"-?\\d+\\.\\d+"};
            std::sregex_iterator pos1(force_string.cbegin(), force_string.cend(), p_floats);
            std::sregex_iterator end1;
            std::vector<double> forces;
            for (; pos1 != end1; ++pos1){
                forces.push_back(stod(pos1->str(0)));
            }
            if (forces.empty()) throw "Cannot find forces";
            return forces;
        }

        std::vector<double> FindFrequencies(const std::string &s){
            std::regex p_frequencies{"\n\\s+Frequencies.*-?\\d+\\.\\d+"};
            std::sregex_iterator pos(s.begin(), s.end(), p_frequencies);
            std::sregex_iterator end;
            std::string freq_string;
            for (; pos != end; ++pos){
                freq_string += pos->str(0);
            }
            std::regex p_floats{"-?\\d+\\.\\d+"};
            std::sregex_iterator pos1(freq_string.cbegin(), freq_string.cend(), p_floats);
            std::sregex_iterator end1;
            std::vector<double> frequencies;
            for (; pos1 !=end1; ++pos1){
                frequencies.push_back(stod(pos1->str(0)));
            }
            if (frequencies.empty()) throw "Cannot find frequencies";
            return frequencies;
        }

        std::vector<double> FindIntensities(const std::string &s){
            std::regex p_intensities{"\n\\s+IR\\s+Inten.*-?\\d+\\.\\d+"};
            std::sregex_iterator pos(s.begin(), s.end(), p_intensities);
            std::sregex_iterator end;
            std::string ir_string;
            for (; pos != end; ++pos){
                ir_string += pos->str(0);
            }
            std::regex p_floats{"-?\\d+\\.\\d+"};
            std::sregex_iterator pos1(ir_string.cbegin(), ir_string.cend(), p_floats);
            std::sregex_iterator end1;
            std::vector<double> intensities;
            for (; pos1 !=end1; ++pos1){
                intensities.push_back(stod(pos1->str(0)));
            }
            if (intensities.empty()) throw "Cannot find intensities";
            return intensities;
        }

        std::vector<double> FindESFrequencies(const std::string &s){
            std::regex p_excited{"\n\\s*Excited\\s+State.*(-?\\d+\\.\\d+)\\s+eV"};
            std::sregex_iterator pos(s.cbegin(), s.cend(), p_excited);
            std::sregex_iterator end;
            std::vector<double> es_freqs;
            for (; pos != end; ++pos){
                es_freqs.push_back(std::stod(pos->str(1)));
            }
            if (es_freqs.empty()) throw "cannot find es_freqs";
            return es_freqs;
        }

        std::vector<double> FindESIntensities(const std::string &s){
            std::regex p_excited{"\n\\s*Excited\\s+State.*f=(-?\\d+\\.\\d+)\\s"};
            std::sregex_iterator pos(s.cbegin(), s.cend(), p_excited);
            std::sregex_iterator end;
            std::vector<double> es_intensities;
            for (; pos != end; ++pos){
                es_intensities.push_back(std::stod(pos->str(1)));
            }
            if (es_intensities.empty()) throw "cannot find es_intensities";
            return es_intensities;
        }

        reparm::Coordinates FindOptCoordinates(const std::string &s){
            /* We're going to have to build a reparm::Coorindate
           object so we need the charge and multiplicity */
            std::regex p_charg_mult{"Charge\\s+=\\s+(\\d+)\\s+M.+(\\d+)"};
            std::smatch m;
            std::regex_search(s, m, p_charg_mult);
            int charge{stoi(m[1].str())};
            int multip{stoi(m[2].str())};


            /* Getting the optimized coordinates is a little harder.
           We split it up into two tasks */

            /* Find the standard orientation blocks */
            std::regex p_stand_orient{"Standard orientation(.|\n)*?(?=Rotat)"};
            std::sregex_iterator pos(s.cbegin(), s.cend(), p_stand_orient);
            std::sregex_iterator end;
            std::vector<std::string> matches;
            for (; pos != end; ++pos)
                matches.emplace_back(pos->str(0));
            auto last_occurance = std::prev(matches.cend(), 1);

            /* Now we extract the coordinates from this block.
           We only want the atomic number, x, y , and z. */
            std::vector<std::vector<float> > v_coordinates;
            std::regex p_coord{"\n\\s+\\d+\\s+(\\d+)\\s+\\d+\\s+(-?\\d+\\.\\d+)"
                                       "\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)"};
            pos = std::sregex_iterator(last_occurance->cbegin(),
                                       last_occurance->cend(),
                                       p_coord);
            for (; pos != end; ++pos){
                float atom_number = std::stof(pos->str(1));
                float x_coord = std::stof(pos->str(2));
                float y_coord = std::stof(pos->str(3));
                float z_coord = std::stof(pos->str(4));
                v_coordinates.emplace_back(std::vector<float>{atom_number,
                                                              x_coord, y_coord, z_coord});
            }

            return reparm::Coordinates{charge, multip, v_coordinates};
        }

        reparm::Parameters FindParameters(const std::string &s){
            std::regex p_parameters{"Method=(.|\n)*?(?=Standard basis)"};
            std::smatch m;
            if (regex_search(s, m, p_parameters))
                return Parameters{m[0]};
            else
                throw "could not read parameters";
            return Parameters();
        }

        std::vector<std::vector<float> > FindNormalModes(const std::string &s){
            /* We first extract the general frequency information into
           a vector of upto three modes per element */
            std::regex p_float{"-?\\d+\\.\\d+"};
            std::regex p_freq_block{"Frequencies(.|\n)*?(?=(Thermo|Frequ))"};
            std::sregex_iterator pos(s.cbegin(), s.cend(), p_freq_block);
            std::sregex_iterator end;
            std::vector<std::string> matches;
            for (; pos != end; ++pos)
                matches.emplace_back(pos->str(0));

            /* Now from each of these elements
           we extract the force contants */
            std::vector<float> force_constants;
            std::regex p_frc_const{"Frc.*"};
            for (const auto i: matches){
                std::smatch m;
                std::regex_search(i, m, p_frc_const);
                std::string match = m[0].str();
                pos = std::sregex_iterator(match.cbegin(),
                                           match.cend(),
                                           p_float);
                end = std::sregex_iterator();
                for (; pos != end; ++pos)
                    force_constants.emplace_back(std::stof(pos->str(0))* 100);
            }

            /* To get the normal modes, we recognize that the rows we
           want from the output are the only ones that begin an integer
           and end with a double */
            std::regex p_atom{"\n\\s+\\d\\s+.*-?\\d+\\.\\d+"};
            /* The normal modes per atom */
            std::vector<std::vector<float> > normals_of_atoms;
            bool first_loop = true;
            for (const auto i: matches){
                pos = std::sregex_iterator(i.cbegin(), i.cend(), p_atom);
                end = std::sregex_iterator();
                std::vector<float> atom_values;
                size_t atom = 0;
                for (; pos != end; ++pos, ++atom){
                    if (first_loop)
                        normals_of_atoms.push_back(std::vector<float>());
                    std::string line = pos->str();
                    /* from each line representing an atom
                       we grab the floats and insert them into
                       the atom vector*/
                    std::sregex_iterator pos1(line.cbegin(), line.cend(), p_float);
                    auto end1 = std::sregex_iterator();
                    for (; pos1 != end1; pos1++)
                        normals_of_atoms[atom].emplace_back(std::stof(pos1->str(0)));
                }
                first_loop = false;
            }

            /* Right now we have two data objects. The first is a simple
           vector of the Force constants per frequency. The second
           is a vector of vectors representing atoms<normal_values>.
           We now want to merge the two such to get a vector of
           vectors, in which the inner vector is composed of
           force_constant, x_atom1, y_atom1, z_atom1, x_atom2 ...
           and the outer represents each frequency*/
            std::vector<std::vector<float> > normal_modes(force_constants.size());
            for (size_t i = 0; i != normal_modes.size(); ++i)
                normal_modes[i].push_back(force_constants[i]);
            for (size_t i = 0; i != normals_of_atoms.size(); ++i){
                std::vector<float> x;
                std::vector<float> y;
                std::vector<float> z;
                for (size_t j = 0; j != normals_of_atoms[i].size(); ++j){
                    if ((j % 3) == 0)
                        x.push_back(normals_of_atoms[i][j]);
                    if ((j % 3) == 1)
                        y.push_back(normals_of_atoms[i][j]);
                    if ((j % 3) == 2)
                        z.push_back(normals_of_atoms[i][j]);
                }
                for (size_t j = 0; j != x.size(); ++j){
                    std::vector<float> v = {x[j], y[j], z[j]};
                    normal_modes[j].insert(normal_modes[j].end(),
                                           v.begin(), v.end());
                }
            }

            return normal_modes;

        }

    }
}
