#P AM1(Input,Print) opt

best

0   1
6        0.925863    -0.772834     0.035697
6       -0.344790    -1.092252     0.021488
8       -1.157321     0.000187    -0.004913
6       -0.317525     1.100371     0.061662
6        0.999822     0.728986     0.085707
1        1.797370     1.383372    -0.018645
1       -0.835531     2.016866     0.010457
1       -0.903272    -2.110048     0.014369
1        1.816842    -1.350601    -0.015067

Method=8 CoreType=1 PeptideFC=0.0121627755 RIJScale=0.5329692868
****
H 
PQN=1 NValence=1 F0ss=0.4596102662 ZetaOverlap=1.1592993284 U=-0.4315264548
Beta=-0.2574815583 CoreKO=1.0589197790 KON=0,0,0,1.0446607980 KON=1,0,1,1.2026196827
KON=0,1,1,0.9852480286 KON=2,1,1,1.0739077844 EISol=-0.4188110042 EHeat=0.0830298228
Alpha=1.3265785197
GCore=0.0066345782,1.4001425905,2.4702535464
GCore=0.0003578457,1.4001425905,3.4015070392
GCore=-0.0013248148,0.5311011039,3.4444930193
****
C 
PQN=2,2 NValence=4 F0ss=0.4280809825 F0sp=0.4354185183 F0pp=0.3443629353 F2pp=0.1874607059
G1sp=0.2978928175
ZetaOverlap=2.0385029058,1.5129290938
U=-1.6710041047,-1.4560985900
Beta=-0.6224486609,-0.3273443342 DDN=0,1,0.7627154330 DDN=1,1,0.7406048937
CoreKO=1.1295849289 KON=0,0,0,1.1424307781 KON=1,0,1,0.7209071901 KON=0,1,1,1.1315783315
KON=2,1,1,0.8096510931 EISol=-4.4398989268 EHeat=0.2723305520 Alpha=1.3390660019
DipHyp=1.5096484532
GCore=0.0008612005,1.5839209328,3.3965438871
GCore=0.0031892455,1.5820975212,3.6106140366
GCore=-0.0014610573,1.5106511485,3.5277311610
GCore=-0.0000912823,1.3076823148,5.3686734941
****
O 
PQN=2,2 NValence=6 F0ss=0.5445226887 F0sp=0.5423941178 F0pp=0.4690460152 F2pp=0.2512047460
G1sp=0.4365302021
ZetaOverlap=3.1537499085,2.5627595232
U=-3.2874974178,-2.9110223154
Beta=-1.1824627399,-1.1994404385 DDN=0,1,0.4533663278 DDN=1,1,0.5416053721
CoreKO=0.8822957477 KON=0,0,0,0.8801278439 KON=1,0,1,0.5396004592 KON=0,1,1,0.8822957477
KON=2,1,1,0.5515584117 EISol=-11.6164441184 EHeat=0.0949133088 Alpha=2.0516420562
DipHyp=0.9977217969
GCore=0.0192698894,1.0058365967,1.6548979063
GCore=0.0071015025,1.9056828561,2.6643272210
****

--Link1--
#P AM1(Input,Print) CIS(Singlets,NStates=10) pop(full) freq

best1

0   1
6        0.925863    -0.772834     0.035697
6       -0.344790    -1.092252     0.021488
8       -1.157321     0.000187    -0.004913
6       -0.317525     1.100371     0.061662
6        0.999822     0.728986     0.085707
1        1.797370     1.383372    -0.018645
1       -0.835531     2.016866     0.010457
1       -0.903272    -2.110048     0.014369
1        1.816842    -1.350601    -0.015067

Method=8 CoreType=1 PeptideFC=0.0121627755 RIJScale=0.5329692868
****
H 
PQN=1 NValence=1 F0ss=0.4596102662 ZetaOverlap=1.1592993284 U=-0.4315264548
Beta=-0.2574815583 CoreKO=1.0589197790 KON=0,0,0,1.0446607980 KON=1,0,1,1.2026196827
KON=0,1,1,0.9852480286 KON=2,1,1,1.0739077844 EISol=-0.4188110042 EHeat=0.0830298228
Alpha=1.3265785197
GCore=0.0066345782,1.4001425905,2.4702535464
GCore=0.0003578457,1.4001425905,3.4015070392
GCore=-0.0013248148,0.5311011039,3.4444930193
****
C 
PQN=2,2 NValence=4 F0ss=0.4280809825 F0sp=0.4354185183 F0pp=0.3443629353 F2pp=0.1874607059
G1sp=0.2978928175
ZetaOverlap=2.0385029058,1.5129290938
U=-1.6710041047,-1.4560985900
Beta=-0.6224486609,-0.3273443342 DDN=0,1,0.7627154330 DDN=1,1,0.7406048937
CoreKO=1.1295849289 KON=0,0,0,1.1424307781 KON=1,0,1,0.7209071901 KON=0,1,1,1.1315783315
KON=2,1,1,0.8096510931 EISol=-4.4398989268 EHeat=0.2723305520 Alpha=1.3390660019
DipHyp=1.5096484532
GCore=0.0008612005,1.5839209328,3.3965438871
GCore=0.0031892455,1.5820975212,3.6106140366
GCore=-0.0014610573,1.5106511485,3.5277311610
GCore=-0.0000912823,1.3076823148,5.3686734941
****
O 
PQN=2,2 NValence=6 F0ss=0.5445226887 F0sp=0.5423941178 F0pp=0.4690460152 F2pp=0.2512047460
G1sp=0.4365302021
ZetaOverlap=3.1537499085,2.5627595232
U=-3.2874974178,-2.9110223154
Beta=-1.1824627399,-1.1994404385 DDN=0,1,0.4533663278 DDN=1,1,0.5416053721
CoreKO=0.8822957477 KON=0,0,0,0.8801278439 KON=1,0,1,0.5396004592 KON=0,1,1,0.8822957477
KON=2,1,1,0.5515584117 EISol=-11.6164441184 EHeat=0.0949133088 Alpha=2.0516420562
DipHyp=0.9977217969
GCore=0.0192698894,1.0058365967,1.6548979063
GCore=0.0071015025,1.9056828561,2.6643272210
****

