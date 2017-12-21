% Zachary Olech, zolech
% 12/13/2017
clear;
clc;

%% Variables
inputFile = 'DATALOG.txt';%input('Please enter the name of the file that you would like to read: ','s');
formatSpec = '%d %s %f %f';
delimiter = ',';

%% Read Input
fileID = fopen(inputFile, 'r', 'n', 'ISO-8859-15');
dataIn = textscan(fileID,formatSpec,'Delimiter',delimiter);
rowNum = length(dataIn{1,1});

%% Process the Data
%Temperature Data
tempTable = zeros(rowNum,5);
tempTable(:,1) = dataIn{1,1}(:,1);  % Index
%tempTable(:,2) = dataIn{1,2}(:,1); % Probe Number <- This is broken
tempTable(:,3) = dataIn{1,3}(:,1);  % Voltage Value
tempTable(:,4) = dataIn{1,4}(:,1);  % Resistance Value

% Calculate All Temperatures
tempTable(:,5) = calcTemp(tempTable(:,4));

%RPM Data
%primaryRPMTable = {1..2,:};
%secondaryRPMTable = {1,:};

%% Display Output
figure('Name', 'Car Data');

% Temperature Probe 0
plot(tempTable(:,1),tempTable(:,5));

%Extra space

%% Functions
function temp = calcTemp( r )

% Values from: http://www.bapihvac.com/wp-content/uploads/2010/11/Thermistor_100K.pdf
tempTable = [ %R Ohms, T Celcius
   8783, 187;
   11949, 171;
   14584, 161;
   17907, 151;
   22111, 141;
   27481, 131;
   34376, 121;
   43273, 111;
   54878, 101;
   70076, 91;
   9028, 81;
   117000, 71;
   153092, 61;
   201971, 51;
   269035, 41;
   361813, 31;
   491217, 21;
   674319, 11;
   935383, 1;
   1000019, -1;
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