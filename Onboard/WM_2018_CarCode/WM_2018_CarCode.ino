/* --- Constants --- */
// sensor value of no magnetic field
#define NO_FIELD = 505L
// Magnetic sensor threshold
#define MAG_THRESH = 10L



/* ---- Basic Variables ---- */
// Mode Variables
boolean testing = false;

// number of over threshold hall readings
int readings;
unsigned long startTime;
unsigned long endTime;
int hallCount = 0;

//Pin Definition
int USBSerial = 57600;  // Speed of USB serial communications
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

/* ---- Additional Functions ---- */
// Function handeling temperature sensor data collection
void temperature() {
  
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
void USBSerial() {

}

/* ---- Primary Loop ---- */
void loop() {
  // Sensor Collection
  temperature();

  // Data Formatting


  // Data Saving (onboard SD Card)


  // Serial Communications
  if (testing == true) {
    USBSerial();
  }
}
