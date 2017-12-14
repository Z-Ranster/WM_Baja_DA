

temp = calcTemp(2.4);

function temp = calcTemp( r )

tempTable = [ %R Ohms, T Celcius
   1, 821;
   54, 252;
   107, 207;
   160, 182;
   213, 165;
   266, 152;
   319, 141;
   372, 131;
   425, 123;
   478, 115;
   531, 107;
   584, 100;
   637, 93;
   690, 86;
   743, 78;
   796, 70;
   849, 60;
   902, 49;
   955, 34;
   1008, 3;
];

i = 1;
    while r < tempTable(i,1)
        i = i + 1;
    end
r1 = tempTable(i,1);
r2 = tempTable(i + 1, 1);
t1 = tempTable(i, 2);
t2 = tempTable(i + 1, 2);
m = (t2-t1)/(r2-r1);
b = t1 - (m * r1);
temp = m * r + b;
end

