/*
 * WM_Onboard_2018
 * Version 1.0
 * Zak Olech
 * 1/26/2018
 */

/*
 * Global Variables
 */
boolean serialComOn = true;
boolean xbeeComsOn = false;

/*
 * Methods
 */
	/*
	 * General
	 */

	/*
	 * Input
	 */
		/*
		 * Temperature
		 */
void setupTemperatureSensors(){

}
void temperatureData(){

}
void getTemperatureData(){

}
void calculateTemperatureData(){

}
		/*
		 * RPM
		 */
void setupRPMSensors(){

}
void rpmData() {

}
void getRPMData() {

}
void calculateRPMValues() {

}
	/*
	 * SD Storage
	 */
void setupSDCard(){

}
void storeData(){

}

	/*
	 * Communication
	 */
		/*
		 * serialCommunication();
		 */
void setupSerialCommunication(){

}
void serialCommunication() {

}
		/*
		 * XBEE Communication
		 */
void setupXbeeCommunication() {

}
void xbeeCommunication() {

}

/*
 * Initialization Method
 */
void setup(){
	/*
	 * General
	 */

	/*
	 * Input Definition
	 */
	setupRPMSensors();
	setupSDCard();
	/*
	 * Output Definitions
	 */
		/*
		 * Communication Definition
		 */
	setupSerialCommunication();
	setupXbeeCommunication();
	/*
	 * Local Storage
	 */
	setupSDCard();
}

/*
 * Main Methods
 */
void main(){
	/*
	 * General
	 */

	/*
	 * Input
	 * Order of Reading
	 * - RPM Data
	 * - Temperature Data
	 */
	temperature();
	rpmData();
	/*
	 * Output
	 */
		/*
		 * Communications
		 */
		serialCommunication();
		xbeeCommunication();
	/*
	 * Local Storage
	 */
	storeData();

}
