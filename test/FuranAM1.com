#P AM1(Input,print) CIS(Singlets,NStates=15) pop(full) freq

AM1-Reparam Input

0   1
6        0.955475   -0.715963    0.000000
6       -0.345627   -1.092818    0.000000
8       -1.157305    0.000000    0.000000
6       -0.345627    1.092818    0.000000
6        0.955475    0.715963    0.000000
1        1.810438    1.369382    0.000000
1       -0.840307    2.046884    0.000000
1       -0.840307   -2.046884    0.000000
1        1.810438   -1.369382    0.000000

Method=8 CoreType=1 PeptideFC=0.0114505045 RIJScale=0.5291772086
****
H
PQN=1 NValence=1 F0ss=0.4721553327 ZetaOverlap=1.1880780000 U=-0.4188110042
Beta=-0.2268825074 CoreKO=1.0589197790 KON=0,0,0,1.0589197790 KON=1,0,1,1.0589197790
KON=0,1,1,1.0589197790 KON=2,1,1,1.0589197790 EISol=-0.4188110042 EHeat=0.0830298228
Alpha=1.5252601686
GCore=0.0085277107,1.4001425905,2.2676713595
GCore=0.0003534810,1.4001425905,3.4015070392
GCore=-0.0012733648,0.5600570362,3.9684248791
****
C
PQN=2,2 NValence=4 F0ss=0.4494442496 F0sp=0.4215147623 F0pp=0.3768030831 F2pp=0.1898715146
G1sp=0.2679025821
ZetaOverlap=1.8086650000,1.6851160000
U=-1.9120180828,-1.4557965594
Beta=-0.5775444233,-0.2836784428 DDN=0,1,0.8236736000 DDN=1,1,0.7268015000
CoreKO=1.1124284736 KON=0,0,0,1.1124284736 KON=1,0,1,0.8219701441 KON=0,1,1,1.1124284736
KON=2,1,1,0.7783928119 EISol=-4.4398989268 EHeat=0.2723305520 Alpha=1.4014062429
DipHyp=1.6472524267
GCore=0.0007885612,1.4001425905,3.0235618126
GCore=0.0031892455,1.4001425905,3.4959933458
GCore=-0.0013931594,1.4001425905,3.8739385724
GCore=-0.0000875022,1.4001425905,5.0077742521
****
O
PQN=2,2 NValence=6 F0ss=0.5666745976 F0sp=0.5321302318 F0pp=0.4958708974 F2pp=0.2358081713
G1sp=0.4343770262
ZetaOverlap=3.1080320000,2.5240390000
U=-3.5951865036,-2.8760896690
Beta=-1.0757546603,-1.0757546603 DDN=0,1,0.4988896000 DDN=1,1,0.4852322000
CoreKO=0.8822957477 KON=0,0,0,0.8822957477 KON=1,0,1,0.5019543089 KON=0,1,1,0.8822957477
KON=2,1,1,0.5515584117 EISol=-11.6164441184 EHeat=0.0949133088 Alpha=2.3576807890
DipHyp=0.9977217969
GCore=0.0195117322,1.4001425905,1.6023328031
GCore=0.0056550009,1.9601996266,2.7307884326
****

