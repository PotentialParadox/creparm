CC = g++-5
CFLAGS = -Wall -std=c++11
INC_DIR = include
VPATH = src:include:bin
OBJECTS = main.o gaussian_input.o coordinates.o parameters.o header.o gaussian.o \
	  parameter_group.o

all : bin/$(OBJECTS)
	$(CC) $(CFLAGS) $^ -o bin/reparm

bin/main.o : main.cpp gaussian_input.h header.h gaussian.h parameter_group.h
	$(CC) -c -I $(INC_DIR) $(CFLAGS) $< -o $@

bin/gaussian_input.o : gaussian_input.cpp header.h coordinates.h parameters.h reparm_exceptions.h
	$(CC) -c -I $(INC_DIR) $(CFLAGS) $< -o $@

bin/coordinates.o: coordinates.cpp
	$(CC) -c -I $(INC_DIR) $(CFLAGS) $< -o $@

bin/header.o: header.cpp
	$(CC) -c -I $(INC_DIR) $(CFLAGS) $< -o $@

bin/parameters.o: parameters.cpp
	$(CC) -c -I $(INC_DIR) $(CFLAGS) $< -o $@

bin/gaussian.o: gaussian.cpp gaussian.h
	$(CC) -c -I $(INC_DIR) $(CFLAGS) $< -o $@

bin/parameter_group.o: parameter_group.cpp gaussian_input.h
	$(CC) -c -I $(INC_DIR) $(CFLAGS) $< -o $@

.PHONY :
clean : 
	rm bin/reparm bin/*.o
