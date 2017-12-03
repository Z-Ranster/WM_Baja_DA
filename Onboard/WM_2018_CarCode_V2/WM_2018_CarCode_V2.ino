/* ---- Basic Variables ---- */
// Mode Variables
boolean testing = true;

//Temperature Variables
#define ThermistorNumber 1   //Number of thermistors in the system
int SeriesResistor = 10000;
float ThermistorRaw[ThermistorNumber];
float ThermistorResistance[ThermistorNumber];
float ThermistorTemp[ThermistorNumber];
//Resistance at 25 Degrees C --> Series Resistor
// temp. for nomial resistance (almost always 25 c)
int TempNomial = 25;
// Beta coefficient of the thermistor (3000-4000)
int BCoEfficient = 3950;

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

/* Background Calculations 
--> Vo=R/(R+10k)*Vcc = R/(R+10k)*3.3 --> Voltage Divider Equation
--> ADC=Vi*1023/Varef
*/

// Multiple Use Variables
int RefVoltage = 5;
int Num_Samples = 5;

//Pin Definition
int USBSerial = 9600;  // Speed of USB serial communications
int ThermistorPin[ThermistorNumber] = {A0};
int hallSensorPin = 0; // pin for hall sensor

/* ---- Initial Setup Code ---- */
void setup() {
  //Pin Definition

  // Serial Communication Setup
  if (testing == true) {
    Serial.begin(USBSerial);
    Serial.println("!-S-USB Serial Started-!");
  }
}

/* ---- Additional Functions ---- */
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

// Function handeling temperature sensor data collection
void temperature() {
  // Temp Variable
  float steinhart;
  int sample_count = 0;
  int i;
  
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
    ThermistorRaw[i] = (ThermistorRaw[i]/sample_count)*(RefVoltage/1023.0);   // Input voltage
    
    //---> This part is broken // Temperature Conversion
    ThermistorResistance[i] = (SeriesResistor)/(1023/ThermistorRaw[i]-1);   //10k / (1023/ADC - 1);   // Resistance
    
    //---> This part is broken // Temperature Conversion
    steinhart = ThermistorResistance[i]/SeriesResistor; // (R/Ro)
    steinhart = log(steinhart); // ln(R/Ro)
    steinhart /= BCoEfficient; // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TempNomial + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart; // Invert
    steinhart -= 273.15; // convert to C
    ThermistorTemp[i] = steinhart;
  }
}

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

// Function handeling serial (USB) communicatoin
// Message Structure !-Type of Message-Message-!
// Type of Message - (S, D) --> S = System Wide; D --> Car Data
void coms() {
  //Loop Variable
  int i;
  
  // Temperature Output
  Serial.println("!-D-Thermistor Data-!");
  for (i = 0; i < ThermistorNumber; i++) {
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
    Serial.print(" Temperature - Fahrenheit: ");
    Serial.println(ThermistorTemp[i]);
  }

  //End of Message Break
  Serial.println("");
}

/* ---- Primary Loop ---- */
void loop() {
  // Sensor Collection
  temperature();

  // Data Formatting


  // Data Saving (onboard SD Card)


  // Serial Communications
  if (testing == true) {
    coms();
  }
}
