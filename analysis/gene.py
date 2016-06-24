import re


class Gene:

    def __init__(self, file_name):
        self.file_name = file_name
        self.input_file = file_name + '.com'
        self.output_file = file_name + '.log'
        self.header = self.__read_header()
        # A tuple of spin, atom_value, coordinates
        self.coordinates = self.__read_coordinates()
        # Params are the chunks of text including the parameter values,
        # and remains constant throughout the program
        self.params = self.__read_parameters()
        # p_floats are the parameter values which will vary among the genes
        self.p_floats = self.__extract_floats()

    def set_p_floats(self, p):
        self.p_floats = p

    def set_file_name(self, name):
        self.file_name = name

    def __read_header(self):
        p_blank = re.compile('^\s*$')
        blank_count = 0
        header_lines = ''
        f = open(self.input_file, 'r')
        for line in f:
            if blank_count < 2:
                header_lines += line
            if re.search(p_blank, line):
                blank_count += 1
            if blank_count == 2:
                break
        f.close()
        return header_lines

    def __read_coordinates(self):
        # Patterns
        p_blank = re.compile('^\s*$')
        p_float = re.compile('\-?\d+\.\d+')
        p_integer = re.compile("\\b(\d+)\s")
        blank_count = 0
        atom_count = 0
        atom_value = []
        coordinates = []
        f = open(self.input_file, 'r')
        for line in f:
            if blank_count == 2 and atom_count == 0:
                spin = line
                atom_count += 1
            elif blank_count == 2 and atom_count > 0:
                if re.search(p_integer, line):
                    m = re.findall(p_integer, line)
                    atom_value.append(float(m[0]))
                if re.search(p_float, line):
                    n = re.findall(p_float, line)
                    for i in n:
                        coordinates.append(float(i))
                atom_count += 1
            if re.search(p_blank, line):
                blank_count += 1
            if blank_count > 2:
                break
        f.close()
        return spin, atom_value, coordinates

    # read_parameters returns a list of [parameter word, line_number,
    # and number of float parameters]
    def __read_parameters(self):
        # Patterns
        p_blank = re.compile('^\s*$')
        p_param = re.compile('\S+')
        p_float = re.compile('\-?\d+\.\d+')
        p_eheat = re.compile('EHeat')
        p_EISol = re.compile('EISol')
        blank_count = 0
        am1_params = []
        f = open(self.input_file, 'r')
        for i, line in enumerate(f):
            if re.search(p_blank, line):
                blank_count += 1
            if blank_count >= 3:
                m = re.findall(p_param, line)
                for element in m:
                    l_floats = re.findall(p_float, element)
                    if (re.search(p_eheat, element) or
                            re.search(p_EISol, element)):
                        l_floats = []
                    am1_params.append([element, i, len(l_floats)])
        return am1_params

    def __extract_floats(self):
        # Patterns
        p_float = re.compile('\-?\d+\.\d+')
        parameter_floats = []
        for parameter in self.params:
            if parameter[2] > 0:
                m = re.findall(p_float, parameter[0])
                for element in m:
                    parameter_floats.append(float(element))
        return parameter_floats