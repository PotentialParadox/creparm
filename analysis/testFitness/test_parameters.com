#P AM1(Input,Print) CIS(Singlets,NStates=10) pop(full)

AM1

0 1
6       -1.284344     0.719484    -0.024738
6       -0.049132     1.320896     0.031053
6       -0.010676    -1.275597     0.031876
6       -1.274743    -0.723173    -0.006274
1       -2.200074    -1.283095     0.077716
1        0.228965    -2.326844    -0.119199
1        0.133067     2.362028     0.036660
1       -2.130213     1.310351    -0.071052
16        1.227367    -0.017618    -0.010012

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
S 
PQN=3,3 NValence=6 F0ss=0.4331396397 F0sp=0.3183640731 F0pp=0.3136271218 F2pp=0.3456917167
G1sp=0.2791629797
ZetaOverlap=2.3665150000,1.6672630000,1.0000000000
U=-2.0834683120,-1.7903186861
Beta=-0.1440781557,-0.2905136336 DDN=0,1,0.9004265000 DDN=1,1,1.0036329000
CoreKO=1.1543033468 KON=0,0,0,1.1543033468 KON=1,0,1,0.8464368816 KON=0,1,1,1.1543033468
KON=2,1,1,0.7746001785 EISol=-7.0460287497 EHeat=0.1058151363 Alpha=1.3026480172
DipHyp=1.8007493954
GCore=-0.0353616377,1.2863644833,1.4563457902
GCore=-0.0008238398,1.6425719595,2.8408498620
GCore=0.0008565489,3.7964407093,3.7967867236
****

--Link1--
#P AM1(Input, Print) freq

AM1

0 1
6       -1.284344     0.719484    -0.024738
6       -0.049132     1.320896     0.031053
6       -0.010676    -1.275597     0.031876
6       -1.274743    -0.723173    -0.006274
1       -2.200074    -1.283095     0.077716
1        0.228965    -2.326844    -0.119199
1        0.133067     2.362028     0.036660
1       -2.130213     1.310351    -0.071052
16        1.227367    -0.017618    -0.010012

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
S 
PQN=3,3 NValence=6 F0ss=0.4331396397 F0sp=0.3183640731 F0pp=0.3136271218 F2pp=0.3456917167
G1sp=0.2791629797
ZetaOverlap=2.3665150000,1.6672630000,1.0000000000
U=-2.0834683120,-1.7903186861
Beta=-0.1440781557,-0.2905136336 DDN=0,1,0.9004265000 DDN=1,1,1.0036329000
CoreKO=1.1543033468 KON=0,0,0,1.1543033468 KON=1,0,1,0.8464368816 KON=0,1,1,1.1543033468
KON=2,1,1,0.7746001785 EISol=-7.0460287497 EHeat=0.1058151363 Alpha=1.3026480172
DipHyp=1.8007493954
GCore=-0.0353616377,1.2863644833,1.4563457902
GCore=-0.0008238398,1.6425719595,2.8408498620
GCore=0.0008565489,3.7964407093,3.7967867236
****


