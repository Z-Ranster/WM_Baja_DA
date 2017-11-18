/* ---- Basic Variables ---- */
// Mode Variables
boolean testing = false;

//Pin Definition
int USBSerial = 57600;  // Speed of USB serial communications

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
// Function handeling temperature sensor data collection
void temperature() {
  
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
