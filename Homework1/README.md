# Házi feladat I.


## Feladat

Implementálja a következő számítást megvalósító függvényt:

Egy számok listájából álló bemenethez megadja annak a listának a maximális pontszámát. A maximális pontszám egy olyan mérték amit a következő `!pszeudó!` lépéssorozat végrehajtásával kaphatunk meg:
- Adott I:=\[x0,x1,...,xN-1\] számok listája
- Adott C, "lépésszámláló", kezdetben C=0
- Adott A, akkumulátor, kezdetben A=0
- Amíg a C < N:
    - Ha C páros legyen A = A + sum(I) egyébként A = A - sum(I)
    - Vegyük el I-ből vagy az első, vagy az utolsó elemet (csak egyet! de azt mindenképpen el kell vennünk, csak az első vagy csak az utolsó elem vehető el!!!)
    - C = C+1
- Az eredmény `A` végső értéke
- **Cél:** `A` legyen maximális értékű!!!
- **Megjegyzés:** a feladat belépési pontja a `auto calc_max_score(std::vector<int> const &in);`  szignatúrájú függvény, ennek nem szabad változnia. Ezen belül a feladat tetszőlegesen strukturálható, osztályok bevezethetőek, segédfüggvények használhatóak, stb.! Az órai anyagokat is lehet használni a megoldáshoz természetesen.

## Példa

```
MaxPont([1,2,4,2,6]) = 12
C = 0
A = 0
I = [1,2,4,2,6]

C:=0 ) A = A + sum([1,2,4,2,6]) {15} ; I = [1,2,4,2]
C:=1 ) A = A - sum([1,2,4,2])   {6}  ; I = [2,4,2]
C:=2 ) A = A + sum([2,4,2])     {14} ; I = [4,2]  vagy I = [2,4]
C:=3 ) A = A - sum([4,2])       {8}  ; I = [4]
C:=4 ) A = A + sum([4])         {12} ; I = []
MaxPont = 12

```
## Tesztadatok

1. Üres bemenet
```
calc_max_score({}) == 0
```

2. Egy element tartozó bemenetek
```
calc_max_score({0}) == 0
calc_max_score({1}) == 1
calc_max_score({-1}) == -1
```

3. Pár triviális bemenet
```
calc_max_score({2, 1}) == 2
calc_max_score({1, 2}) == 2
calc_max_score({1, 2, 3}) == 5
```

4. Bonyolódik...
```
calc_max_score({1, 2, 4, 2, 6}) == 12
calc_max_score({8, 7, 4, 2, 5}) == 20
```

5. Stressz
```
calc_max_score({ 331, -813,  380,  208,  352,  110, -801, -675,
                 916, -480, -448,  616,  785,  275,  894,  987,
                -160,  959,  459,  900,  597, -701, -154, 310}) == 8134
```

6. Stressz még nagyobb bemenetekkel
```
calc_max_score({ 912,  662,  244, -356,   96,  184, -491, -535, -407,  185,  352,
                 668, -498,  814, -517, -569, -568,  294, -160,  908,  717,  -22,
                 690, -194,  201,  542,  762, -194,  -17,  143, -585,   49,  114,
                 255, -247,  491, -709,  910,  586, -255,   -2,   -9,  465,  528,
                -650, -843,  946, -553,  451,  985, -998, -354, -173, -166, -370,
                 274, -888, -285,   58, -401,  537,  702,  473,  754}) == 15863
```

7. Ulti
```
calc_max_score({ -189,  -308,    28,  -505,  -31,  -814, -50,   -858, -235,  611,
                  825,    46,   497,  -296,  227,  -627, -90,   -481, -807,  -324,
                  278,  -546,  -866,   219,   956,  -198, -1018, 170,  317,   652,
                   83,   285,  -383,   755,   156,  -380, -240,  -956, 225,   568,
                 -969,   -56,   302,  -884,  565,  -882, -889,  724,  -46,   723,
                  114,   932,   475,  -411,  828,  -701, 672,   -540, 60,    902,
                 -857,   788,  -772,  -596,  -803, 898,  296,   -788, 513,   -265,
                  571,   900,  -917,  -941,  -349, -2,   10,    334,  689,   370,
                 -674,   -33, -1020,  -618,  -808, 881,  -1016, -114, 414,   773,
                 -905,  -982,  -236,   263,   -472, -719, 289,   -682, 957,   -39,
                  368,  1015,  -407, -1014, 786,  623,  262,   -696, -401,  -41,
                  233,  -583,   -57,   -73,   843,  -174, 92,    -111, 369,   195,
                 -702,   341,   986,  -782,  -832, -570, -24,   -576, -256,  -898,
                 -422,   634,  -954,  -104,  -197, 998,  944,   -508, 918,   564,
                  821,   995,   609,   127,   1019, -993, 184,   25,   -847,  710,
                  115,   274, -1001,   872,   787,  -582, -655,  776,  -680,  580,
                  415,  1021,    31,   378,   -119, 423,  -123,  402,  190,   -911,
                 -502,   730,   -18,  -115,  -746, -156, 287,   205,  -681,  -963,
                  325,   964,  -791,  -988,  884,  326,  594,   646,  482,   -329,
                  925,  -125,  -868,  -773,  -152, -684, 725,   -165, -190,  176,
                 -910,    91,   938,  -448,  447,  213,  -92,   816,  179,   -373,
                 -455,  -386,   172,   759,   916,  -703, -281,  -63,  -736,  -551,
                  556,   512,  -818,   476,   -98,  141,  307,   -939, 589,   50,
                 -167,   169,   153,  -229,  -462, 309,  588,   -364, -552,  -713,
                   58,   813,   217,    38,    559,  685,  -331,  84,   -327,  -789,
                   41,  -781,   658,   108,   -725, -654, -354,  215,  -802,  620,
                  926,  -538,   636,   661,   -372, -728, 14,    -29,  -897,  -532,
                 -141,   974,  -824,  -231,  -173, -264, -678,  -687, 430,   -395,
                 -948,   618,   893,    27,    6,    -501, -357,  -325, -794,  252,
                  162,   852,  -529,  -621,  481,  -136, -403,  -541, 708,   979,
                  411,  -999,   104,   261,   865,  -593, -877,  -641, -769,  962,
                -1010,   -81,  -770,    59,    698,  15,   295,   -249, -700,  808,
                  331,   975,  -881,   332,   -490, 945,  931,   -9,   45,    577,
                  584,   432,  -602,  -809,  988,  891,  -436,  -374, 858,   -389,
                 1001,   936,  -699,  -211,  -361, 677,  923,   327,  621,   -185,
                 -385,  -451,   996,  -783,  -587, 842,  -323,  -778, -293,  -272,
                 -677,  -202,  -461,  -604,  1006, -154, 355,   -248, -536,  216,
                 -990,  -965,  -871,   774,   567,  329,  504,   -828, 477,   282,
                -1002,   847,  -649,   -55,   -856, -610, -1023, -445, 340,   -628,
                  -38,  -335,   472,   791,   -757, -306, -340,  -543, 833,   951,
                 -474,   323,   258,  -571,  335,  -334, 655,   -206, 397,   674,
                  438,  -647,   595,  -171,  731,  625,  -99,   101,  -635,  -569,
                  795,   955,   256,   929,   272,  761,  393,   669,  780,   -652,
                 -987,   357,   138,   -75,   269,  354,  -764,  726,  406,   -26,
                 -175,   757,   122,  -707,  578,  321,  -572,  -970, -944,  545,
                 -158,   939,  -636,    21,    -756, -920, 555,   1008, -745,  100,
                  691,  -669,   -11,  -132,  505,  546,  -290,  1,    610,   840,
                  528,    13,   605,  -961,  147,  635,  899,   -121, 48,    -744,
                 -656,   -88,   940,  -565,  462,  937,  -556,  -500, -978,  450,
                 -353,  -499,  -250,   279,   -393, 125,  -942,  -107, 533,   -981,
                 -806,   876,   744,   592,   -476, -78,  -406,  -901, 800,   -995,
                -1015,   563,   638,  -410,  70,   47,   165,   -457, 585,   253,
                  914,  -427,  -879,  -949,  424,  -28,  -271,  -935, -210,  -588,
                  471,   228,   980,   573,   -260, 421,  -129,  -87,  -608,  841,
                 -595,   401,  -176,  -491,  -326, 146,  -148,  484,  -985,  -409,
                 -124,   346,  -724,   168,   -710, -846, 721,   -665, -971,  -328,
                -1003,  -351,    22,  -589,  359,  -321, -82,   -144, 665,   501,
                  576,  -855,   604,  -694,  1018, -921, -928,  854,  -915,  -313,
                   26,  -178,  -929,  -241,  -525, 848,  -1009, -27,  -810,  -342,
                 -194,  -514,  -533,  -908,  383,  810,  767,   -217, 574,   831,
                 -183,  -192,  -130,  -771,  312,  -600, 961,   -504, 86,    338,
                 -831,   970,    39,  -513,  815,  511,  -753,  -212, -118,  -799,
                 -463,   485,  -318,   715,   544,  440,  89,    579,  387,   142,
                  587,   436,   738,    23,    -301, -530, -122,  720,  -886,  624,
                  794,  -888,  -458,  -495,  -991, 367,  -805,  -912, 514,   -613,
                 -775,   983,   997,    24,    -58,  375,  711,   596,  480,   1016,
                 -716,   529, -1004,   921,   -113, 82,   75,    382,  535,   -848,
                 -704,   -34,   657,    62,    -750, -616, 912,   739,  245,   -833,
                  173,  -423,   777,  1004,  -408, -804, -737,  -470, -964,  -333,
                 -120,   192,  -243,   643,   -44,  -127, 333,   -218, 798,   -515,
                 1013,   288,  -762,  -428,  -742, 681,  -511,  313,  223,   553,
                 -289,  -984,   642, -1024, 455,  890,  -1011, 157,  -322,  -749,
                 -477,  -378,    -4,  -464,  729,  187,  -715,  442,  -355,  -396,
                  994,  -594,   930,   350,   -352, -720, -201,  -425, 154,   -343,
                  244,   536,   230,  -979,  -545, 408,  -598,  895,  105,   838,
                 -760, -1007,  -754,    69,    -300, 645,  -648,  -813, -278,  561,
                   29,  -134,   -43,  -867,  250,  -815, -177,  109,  55,    381,
                 -430,   768,   796,   826,   -934, 299,  -662,  662,  420,   -865,
                   33,  -940,  -381,  -644,  700,  -49,  -101,  -291, -449,  -379,
                  232,  -390,   860,  -315,  -40,  -80,  347,   -59,  124,   -358,
                 -586,  -535,  -835,   235,   -61,  917,  466,   330,  575,   756,
                  583,  -727,   582,   542,   -375, -71,  155,   -465, -730,  586,
                  516,   433,  -676,   751,   222,  -89,  704,   94,   182,   824,
                  158,   291,   933,   659,   -625, 985,  194,   -12,  -574,  -257,
                  498,  -836,    36,   549,   806,  -498, 12,    -25,  113,   -366,
                   76,  -893,   303,  -446,  -992, 366,  474,   551,  -286,  -938,
                  310,   849,   943,  -637,  766,  -359, 388,   722,  -601,  -7,
                 -959,  -660,  -580,   316,   204,  -181, 973,   -506, -413,  -689,
                  572,  -164,   224,   418,   -435, 78,   40,    324,  221,   -304,
                  679,  -223,   901,   863,   728,  -617, 606,   971,  958,   -998,
                   35,   984,  -792,  1020,  -679, 209,  -337,  -577, -62,   134,
                  888,  -452,   732,   152,   -844, -97,  -688,  241,  -534,  630,
                 -345,    20,  -245,    -1,    670,  822,  -555,  896,  52,    344,
                 -441,  -983,   889,   782,   942,  856,  -166,  878,  77,    -299,
                 -951,   371,   690,   356,   538,  812,  -591,  88,   -215,  -384,
                  377,  -208,    98,  -434,  -263, 360,  -486,  -344, -1012, -560,
                 -708,  -199,   853,    53,    -346, 318,  404,   37,   140,   772,
                  284,   885,  -667,  -362,  541,  701,  257,   128,  308,   -6,
                 -382,   419,  -279,  -820,  495,  799,  827,   -947, -651,  -460,
                  517,  -630,   602,   707,   -919, -17,  380,   49,   426,   801,
                  959,  -721,  1007,  -967,  -128, 121,  251,   -443, -554,  -404,
                 -892,  -140,   164,  -873,  -102, -507, 16,    683,  -214,  -931,
                  522,   873,  -282,  -825}) == 248924
```