# AM1 Reparameterization
AM1 Reparameterization is an atempt to automate the creation of parameters for specific molecular classes.
## Prerequisites
Users must have a working copy of gaussian 09, cmake (version > 3), g++ >= 4.9 or clang >= 3.5.
## Installation
Clone the git. Create a build folder. Install.
```
git clone https://github.com/PotentialParadox/creparm
mkdir bin
cd bin
cmake ..
make install
```
## Usage
Create a new folder in which you will run the optimization.
Place gaussian input file with cartesian coordinates into the folder.
The atom types can be either character or numeric.
An possible input could be
```
#P AM1 opt

Thiophene

0 1

C       0.8     -0.68   -0.03
C      -0.4     -1.23    0.01
C      -0.36     1.28    0.06
C       0.89     0.74   -0.01
H       1.78     1.32   -0.02
H      -0.57     2.32    0.20
H      -0.62    -2.27    0.12
H       1.78    -1.27    0.06
S      -1.61     0.04   -0.13

```
Inside this same folder you need to create an input file called reparm.in.
Here is an example
```
Input File:                Thiophene.com
High Level Theory          B3LYP/3-21G
Number of Excited States   10
Number of Generations      200
Number of Processors       4
Number of Geometries       128
Number of Elites           2
Population Size            8
Mutation Rate              0.10
Mutation Perturbation      0.05
Survival Chance            0.5
Geometric Perturbation     0.10
```
The number of elites represent how many members of a generation you want to mutate only if their mutation is an improvement.
