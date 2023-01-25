#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>

#define Threshold 80 // A fire is indicated from this value.

SoftwareSerial GSM(7, 8); //SIM800L Tx & Rx is connected to Arduino #7 & #8

#define MQ2pin 0
// Variables
String fileName = "/test.csv"; // PUT THE NAME OF THE FILE ON YOUR SD CARD HERE
const char number[] = "+XXXXXXXXXXXXX"; // PUT YOUR PHONE NUMBER HERE WITH THE CORRESPONDING COUNTRY CODE

File myFile;
int counter =0; // Counter for sensor values above threshold
int counterThreshold = 20; // Threshold to catch false positives

char text[100];
String timeStamp;
bool error; 
bool sent = false;

int sensorValue;  // This value indicates Rs/Ro
const unsigned int wt_ms = 200; // Wait ms
// Methods
void initSD(){ //Initializes SD Card Module

  Serial.print("Initializing SD-Card: ");

  if(!SD.begin(10)) {
    Serial.println("SD Card initialization failed");
    while(1);
  }

  Serial.println("SD Card initialization successful");
}

void writeSDHeader() { //Writes Headers into SD File
  myFile = SD.open(fileName, FILE_WRITE);

  if(myFile) {
    Serial.println("--------------------------\n");
    Serial.print("Writing to csv file: ");

    myFile.println("Date,Time,SensorValue");

    myFile.close();
  }
  else {
    Serial.println("error opening csv file");
  }
}

void writeSDData() { //Writes the recieved values in a CSV with timestamp and the respective value
  myFile = SD.open(fileName, FILE_WRITE);

  if(myFile) {
    myFile.print(timeStamp);
    myFile.print(", ");
    myFile.print(sensorValue);
    myFile.println();

    myFile.close();
  }
  else if (!myFile) {
    Serial.println("error opening csv file");
  }
}

void getTime() { // Gets the timestamp vom GSM module and crops the string to the needed format
  GSM.println("AT+CCLK?");
  timeStamp = "";
  delay(200);
  if(GSM.available()) {
    timeStamp = GSM.readString();
    delay(100);
  }
  timeStamp.remove(0, 19);
  timeStamp.trim();
  timeStamp.remove(20);
  timeStamp.trim();
  return timeStamp;
}

void updateSerial(unsigned int wait_ms){ // Needed for communication with the SIM Module
  String dataString = "";
  delay(wait_ms);
  if(GSM.available()) {
    dataString = GSM.readString();
    Serial.println(dataString);
  }
  while(Serial.available()) {
    GSM.write(Serial.read());
  }
}

void initSIM() { //Initializes SIM Module

  GSM.begin(9600);

  GSM.println("AT"); //SIM_Modul Handshake test.
  updateSerial(wt_ms);

  GSM.println("AT+CLTS = 1"); // Make the SIM Module log the time
  updateSerial(wt_ms);

  GSM.println("AT&W"); // Copy the last input so timelogging statis is not lost on restart.
  updateSerial(wt_ms);

  GSM.println("AT+CFUN=1,1"); // Restart the SIM Module so the timelogging can work
  updateSerial(wt_ms);

  GSM.println("AT"); //SIM_Modul Handshake test.
  updateSerial(wt_ms);

  delay(10);

  GSM.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial(wt_ms);

  delay(10);	

  GSM.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial(wt_ms);

  delay(10);

  GSM.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial(wt_ms);

  GSM.println("AT+CLTS = 1"); 
  updateSerial(wt_ms);

  delay(10);

  GSM.println("AT+CCLK?"); // Requests Time
  updateSerial(wt_ms);

}

void sleepSIM() { // Sets SIM Module in Sleep Mode
  GSM.println("AT+CSCLK=2");
  updateSerial(wt_ms);
  delay(6000);
  Serial.println("SIM_Module in temporary sleep mode");
}

void allWorkingSMS () { // Sends an SMS once everything is set up correctly and all modules are initialized.GSM.begin(9600);
  GSM.begin(9600);
  GSM.println("AT"); //Command to wake up SIM_Module
  updateSerial(wt_ms);
  GSM.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial(wt_ms);
  GSM.println("AT+CMGS=\""+String(number)+"\""); 
  updateSerial(wt_ms);
  GSM.print("All Modules have been correctly initialized and the Gas Sensor is warmed up. Forrest Fire Alarm is now live!");
  GSM.write(26);
  GSM.println("");
  updateSerial(wt_ms);
}

//Code
void setup() {

	Serial.begin(9600);
	delay(3600000);  // Warmup delay, best to be around an hour

  Serial.println("Warmed up.");

  initSIM(); 
  //sleepSIM();

  initSD();
  writeSDHeader();
  delay(2000);

  allWorkingSMS();

  delay(5000);
}

void loop() {

  getTime();  //Get the timestamp
  Serial.println(timeStamp);

  sensorValue = analogRead(MQ2pin); //Read value from MQ2 sensor
  
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  writeSDData(); //Writes sensor value and timestamp in CSV

  if(sensorValue > Threshold) { // If the sensor value is above the threshold there might be a fire
    counter++; // Counter for the number of limit value exceedances
    if (counter > counterThreshold) { // If limit value was exceeded, send Warning SMS
      Serial.print("Smoke detected!");
      if (!sent){ // Check if warning SMS was already sent
        GSM.begin(9600);
        sprintf(text, "Sensor Value of %d detected. There is likely a forest fire at location of sensor ID 1.", sensorValue);
        GSM.println("AT"); // Command to wake up SIM_Module
        updateSerial(wt_ms);
        GSM.println("AT+CMGF=1"); // Configuring TEXT mode
        updateSerial(wt_ms);
        GSM.println("AT+CMGS=\""+String(number)+"\"");
        updateSerial(wt_ms);
        GSM.print(text);
        GSM.write(26);
        GSM.println("");
        updateSerial(wt_ms);
        sent = true;
        counter = 0;
      }
    }  
  }
  else if (sensorValue <= Threshold) { // Resets the Counter if the sensor value is again under the threshold value
    counter = 0;
  }
  delay(3000);
}
