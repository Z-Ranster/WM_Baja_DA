/* ---- Includes ---- */
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

/* ---- Basic Variables ---- */
// Mode Variables
boolean testing = true;  //This gives true=more/false=less data via serial communication (set to false to do live data in Matlab) <-- False is currently broekn
boolean logging = true;
boolean wirlessEnables = false;

//Data Logger Shield Variables
const int chipSelect = 10;  // This is the chip set to use for an Adafriud SD Shield - This is also the digital pin being used
String delimiter = ", ";
boolean SDExists = false;
boolean fileSucessfullySaved = false;
String fileName = "dataLog.csv";
File myFile;

//Temperature Variables
#define ThermistorNumber 1   //Number of thermistors in the system
int SeriesResistor = 10000;  //Measured in ohms
float ThermistorRaw[ThermistorNumber];
double ThermistorResistance[ThermistorNumber];
double ThermistorTemperature[ThermistorNumber];

/*
//RPM Variables
// number of over threshold hall readings
int readings;
unsigned long startTime;
unsigned long endTime;
int hallCount = 0;
// sensor value of no magnetic field
int NO_FIELD = 505;
// Magnetic sensor threshold
int MAG_THRESH = 10;

//Telemetry Variables

*/

// Multiple Use Variables
int Num_Samples = 5;  // This value is used for averaging in multiple functions
int primaryLoopDelay = 1000;  //This is the 1 second loop placed on the primary loop
int loopNum = 0;      // This variable is used to count the number of loops so itme can be estimated

//Pin Definition
int USBSerial = 9600;  // Speed of USB serial communications
int ThermistorPin[ThermistorNumber] = {A0};
int hallSensorPin = 0; // pin for hall sensor
int isDataWritingNo = 3;  //If this is on then the system is not recording data
int SDNoError = 4; //If this is on then there is no error in the SD Card System

/* ---- Initial Setup Code ---- */
void setup() {
  //Pin Definition
  pinMode(isDataWritingNo, OUTPUT);
  pinMode(SDNoError, OUTPUT);
  
  // Serial Communication Setup
  if (testing == true) {
    Serial.begin(USBSerial);
    Serial.println("!-S-USB Serial Started-!");
  }

  // Logging File Setup
  Serial.println("!-S-Initalizing SD card-!");
  if(SD.begin(chipSelect))
  {
    //Card exists and everything is good
    myFile = SD.open(fileName, FILE_WRITE);
    myFile.println("");
    myFile.close();
    SDExists = true;
    digitalWrite(isDataWritingNo,HIGH);
    digitalWrite(SDNoError,HIGH);
  }
  else
  {
    //Error Handling
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    //Card does not exist and everything is not good
    digitalWrite(isDataWritingNo,HIGH);
    digitalWrite(SDNoError,LOW);
  }
  
}

/* ---- Variable Definition Post Setup ---- */
//SD Card Data

/* ---- Additional Functions ---- */
/*
// returns 0 for no field, negative for south pole, positive for north pole
int hallSense(){
  // sensor measurement (add correct input port)
  int raw = analogRead(hallSensorPin);

  long scaledMeasurement = raw - NO_FIELD;

  if(scaledMeasurement < MAG_THRESH && scaledMeasurement > -MAG_THRESH){
    return 0;
  } else {
    return scaledMeasurement;
  }
}

*/

//Calculate Temperatures
//Temperatures measured in Degree of Farenheit
double calcTemp(double ohms)
{
  // Values from: http://www.bapihvac.com/wp-content/uploads/2010/11/Thermistor_100K.pdf
  /*refOhmTable = [ //R Ohms, T Farenheit
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
     90208, 81;
     117000, 71;
     153092, 61;
     201971, 51;
     269035, 41;
     361813, 31;
     491217, 21;
     674319, 11;
     935383, 1;
     1000019, -1;
  ];*/

  // Variables
  int len = 20;
  double refOhmsTable[len] = {8783, 11949, 14584, 17907, 22111, 27481, 34376, 43273, 54878, 70076, 90208, 117000, 153092, 201971, 269035, 361813, 491217, 674319, 935383, 1000019};
  signed int refTempTable[len] = {187,171,161,151,141,131,121,111,101,91,81,71,61,51,41,31,21,11,1,-1};
  double b = 0.0; // Temp
  double m = 0.0; // Temp/Ohms
  double temp = 0.0;
  int i = 0;
  
  // Calculate Temperatures
   while(ohms>refOhmsTable[i])
   {
     i++;
   }
   //P1(rl, th) P2(rh,tl) --> [Ohms, Temp]
   double point1[] = {refOhmsTable[i],refTempTable[i]};
   double point2[] = {refOhmsTable[i+1],refTempTable[i+1]};
   m = (point2[1]-point1[1])/(point2[0]-point1[0]);
   b = point1[1]-(m*point1[0]);
   temp = m*ohms+b;

   //Return Data
   return(temp);
}

// Function handeling temperature sensor data collection
void temperature() {
  // Temp Variable
  int sample_count = 0;
  int i;
  float part1;
  float part2;
  
  // Get Data
  for (i = 0; i < ThermistorNumber; i++) {
    while (sample_count < Num_Samples) {
      ThermistorRaw[i] += analogRead(ThermistorPin[i]);
      sample_count++;
    }
  }

  //Calculate Values
  for (i = 0; i < ThermistorNumber; i++) {
    //---> Voltage
    ThermistorRaw[i] = ((ThermistorRaw[i]/sample_count)*(5.13/1023.0));   // Input voltage --> Ref voltage 5.13 this is a measured value
    //---> This part is broken // Temperature Conversion
    part1 = (SeriesResistor*ThermistorRaw[i]);
    part2 = (ThermistorRaw[i]-5.13);
    ThermistorResistance[i] = -1*(part1/part2);    //Modified voltage division equation to figure out the resistance of R1
    //---> Temperature
    Serial.println("Calculating Temperature");
    ThermistorTemperature[i] = calcTemp(ThermistorResistance[i]);
  }
}

/*

// functions for getting rpm from hall sensor
void logHallSensor(){
  if(hallSense != 0){
    readings++;
  }
  hallCount++;
}
void resetRPM(){
  hallCount = 0;
  startTime = millis();  
}

int calculateRPM() {
  endTime = millis();
  double elapsedTime = (endTime - startTime) / 1000.0;
  double rounds = readings / 2;
  double roundsPerSecond = rounds/elapsedTime;
  return (int) roundsPerSecond * 3600;  
}

*/

// Function handeling serial (USB) communicatoin
// Message Structure !-Type of Message-Message-!
// Type of Message - (S, D) --> S = System Wide; D --> Car Data
void comsTesting() {
  //Loop Variable
  int i;

  //Header
  Serial.print("! - Loop: ");
  Serial.println(loopNum);

  //Data
  
  // Temperature Output
  Serial.println("!-D-Thermistor Data-!");
  for (i = 0; i < ThermistorNumber; i++) {
    Serial.print("Thermistor ");
    Serial.println(i);
    Serial.print("Thermistor ");
    Serial.print(i);
    Serial.print(" Raw - Volts: ");
    Serial.println(ThermistorRaw[i]);
    Serial.print("Thermistor ");
    Serial.print(i);
    Serial.print(" Resistance - Ohms: ");
    Serial.println(ThermistorResistance[i]);
    Serial.print("Thermistor ");
    Serial.print(i);
    Serial.print(" Temperature - Farenheit: ");
    Serial.println(ThermistorTemperature[i]);
  }

  //System Status
  
  //Status of Data Writing
  Serial.println("!-S-SD Card-!");  
  Serial.print("SD Exists: ");
  if(SDExists){
    Serial.println("true");
  }
  else
  {
    Serial.println("false");
  }
  if (fileSucessfullySaved)
  {
    Serial.println("Card Status: Data Saved");
  }
  else
  {
    Serial.println("Card Status: Data Not Saved");
  }
  
  //End of Message Break
  Serial.println("");
}

/*
//Serial Coms used for live data
void comsNonTesting() {
  //Loop Variable
  int i;

  //Header
  Serial.print("*");
  Serial.print(loopNum);
  Serial.print(delimiter);

  //Data
  
  // Temperature Output
  for (i = 0; i < ThermistorNumber; i++) {
    Serial.print("TID");  //Thermistor Index
    Serial.print(i);
    Serial.print(delimiter);
    Serial.print("V");
    Serial.print(ThermistorRaw[i]);
    Serial.print(delimiter);
    Serial.print("R");
    Serial.print(ThermistorResistance[i]);
    Serial.print(delimiter);
    Serial.print("T");
    Serial.print(ThermistorTemperature[i]);
    Serial.print(delimiter);
  }

  //System Status
  
  //Status of Data Writing
  Serial.print("SD");
  Serial.print(delimiter);  
  Serial.print("P");
  Serial.print(delimiter);
  if(SDExists){
    Serial.println("1"); // Exists
  }
  else
  {
    Serial.println("0"); //Does not exist
  }
  Serial.print(delimiter);
  Serial.print('S');
  Serial.print(delimiter);
  if (fileSucessfullySaved)
  {
    Serial.println("1");  // Data saved
  }
  else
  {
    Serial.println("0");  // Data not saved
  }
  Serial.print(delimiter);
  //End of Message Break
  Serial.println("");
}
*/

//Save Data
void saveData(){
  //Variables
  int i = 0;
  fileSucessfullySaved = false;

  //Check to see if the file still exists and open it if it does
  if(myFile = SD.open(fileName, FILE_WRITE))
  {
    //LoopIndex
    myFile.print(loopNum);
    myFile.print(delimiter);
    
    //Add Thermistor Data
    for (i = 0; i < ThermistorNumber; i++) {
      myFile.print("Thermistor_");
      myFile.print(i);
      myFile.print(delimiter);
      myFile.print(ThermistorRaw[i]);
      myFile.print(delimiter);
      myFile.print(ThermistorResistance[i]);
      myFile.print(delimiter);
      myFile.print(ThermistorTemperature[i]);
      myFile.print(delimiter);
    }
    
    //Add RPM Data
  
    //Start new row and close the file
    myFile.println("");
    myFile.close();
  
    //Chage system states to show that everything is all good and writing
    digitalWrite(isDataWritingNo,LOW);
    fileSucessfullySaved = true;
  }
  else
  {
    //File could not be opened
     digitalWrite(isDataWritingNo,HIGH);
     digitalWrite(SDNoError,LOW);
  }
}

/* ---- Primary Loop ---- */
void loop() {
  //Reset Loop Num at 10000
  if(loopNum==10000)
  {
    loopNum = 0;
  }
  
  // Sensor Collection
  temperature();

  // Data Formatting


  // Data Saving (onboard SD Card)
  if (SDExists == true) { 
    saveData();
  }

  // Serial Communications
  if (testing == true) {
    comsTesting();
  }
  else
  {
    /*
    comsNonTesting(); <-- Finish This
    */
  }

  //Delay and increase loop num
  delay(primaryLoopDelay);
  loopNum++;
}
