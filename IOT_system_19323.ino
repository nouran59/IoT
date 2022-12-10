#include <ThingSpeakWriter_asukiaaa.h>
#include <ThingSpeak.h>
#include <Esp32WifiManager.h>
#include <WiFiLink.h>
//ThingSpeak cloud code:
#include <ESP32WiFi.h>
String apiKey = "ZE8ZGF6GBEZUKNT4";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "WE";     // replace with your wifi ssid and wpa2 key
const char *pass =  "10004000"; //replace with your network password 
const char* server = "api.thingspeak.com";
WiFiClient client;
//---------------------------------------------------------------
//Temperature sensor:
//Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 22

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;
 float tempC;
//variables needed decleration:  :
float mass = 960; //mass
const float width = 10; //width of the container
float Na = 6.022 * pow(10, 23); //avogadro's number
float M = (Na * mass); //M = (6.022 * (10 ^ 23) * mass);
const float R = 0.082; //Gas constant
float density = 0; //initial value of density 
double Molecular_velocity = 0.0; //initial value of Molecular_velocity
//dividing parts of Molecular_velocity equation on multiple variables 
float variable1 = (3 * R); 
float variable2 = (google / tempC);
float variable3 = (power /M);
float variable4 = sqrt(tired);
/*
   Setup function. Here we do the basics
*/
//Ultrasonic sesnor variable decleration
long duration; // variable for the duration of sound wave travel
float distance; // variable for the distance measurement
float height; //calculated as the difference between the elevation at which the sensor is fixed at and the distance
float volume; //from its equation (volume = width * width * height )
const float trigPin = 5;  // OUTPUT pin of ultrasonic 
const float echoPin = 18; // INPUT pin of ultrasonic 
//------------------------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  // initialize serial communications / port at 115200 bps:
  Serial.begin(115200);
  Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
//-----------------------------------------------------------------
//ultrasonic sensor requirements:
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
//-----------------------------------------------------------------
  //temperature sensor requirements:
  Serial.println("Dallas Temperature IC Control Library Demo");

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");


  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");

  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
//  printAddress(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC);
  Serial.println();
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  // method 1 - slower
  Serial.print("Temp C: ");
  Serial.println(sensors.getTempC(deviceAddress));
  delay(1000);
//  Serial.print("Temp F: ");
//  Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit

  // method 2 - faster
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == DEVICE_DISCONNECTED_C)
  {
    //Serial.println("Error: Could not read temperature data");
    return;
  }
}
/*
   Main function. It will request the tempC from the sensors and display on Serial.
*/


void loop() {
//Related to ThingSpeak
if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(tempC);
                             postStr +="&field2=";
                             postStr += String(density);
                             postStr +="&field3=";
                             postStr += String(Molecular_velocity);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
// wait 2 seconds before the next loop for the analog-to-digital
//  delay(2000);
//-------------------------------------------------------------------
//ultrasonic sensor:
// Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1500);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1500);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  height = 15 - distance;
  Serial.print("Height: ");
  Serial.println(height);
  volume = width * width * height;
  Serial.print("volume: ");
  Serial.println(volume);
  delay(2000);
  density = (mass/ volume);
  Serial.print("Density:  ");
  Serial.println(density);

//temperature sensor:
// call sensors.requestTemperatures() to issue a global temperature 
// request to all devices on the bus
  Serial.print("Requesting temperatures...");

  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  // It responds almost immediately. Let's print out the data
  printTemperature(insideThermometer); // Use a simple function to print out the data
  
  //equations:
  Serial.println("......"); //to male just a seperate line
  Serial.println("M:  ");
  Serial.print(M / pow(10, 20));
  delay(5000);
  variable1 = (3 * R);
  Serial.println("variable1 :");
  Serial.print(variable1);
  variable2 = (variable1 * tempC);
  Serial.println("variable2 :" );
  Serial.print(variable2);
  variable3 = (variable2 /M);
  Serial.println("variable3 :" );
  Serial.print(variable3);
  variable4 = sqrt(variable3);
  Serial.println("variable4 :" );
  Serial.print(variable4 ,17);
  //after division calculte Molecular_velocity
  Molecular_velocity = sqrt( ( 3 * R * tempC) / (M));
  Serial.println("Molecular_velocity: ");
  Serial.print(Molecular_velocity ,17);
  
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }

}
