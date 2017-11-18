/******************************************

*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
/* 
 *  
 #include <Wire.h>
#include <LiquidCrystal_I2C.h>
*/

// prototypes
boolean connectWifi();

//on/off callbacks
void BedroomLightOn();
void BedroomLightOff();
void BedroomFanOn();
void BedroomFanOff();
void BedroomLampOn();
void BedroomLampOff();
void outletTwoOn();
void outletTwoOff();

// Change this before you flash
const char* ssid = "AR5511_6B4808"; 
const char* password = "23456789";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *BedroomLight = NULL;
Switch *BedroomFan = NULL;
Switch *BedroomLamp = NULL;
Switch *outletTwo = NULL;

// Set Relay Pins
int relayOne = 16;
int relayTwo = 05;
int relayThree = 04;
int relayFour = 00;

/*
 * 
 // Addr: 0x3F, 20 chars & 4 lines. Sometimes display boards use address 0x27
LiquidCrystal_I2C lcd(0x3F, 4, 20); //Frentally display, use 0x3F if not working try 0x27

*/
void setup()
{
  /*
  //Initalize LCD
  lcd.init();
  lcd.noBacklight();
  lcd.backlight();
  lcd.begin(20, 4);
  */
  //Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();
  //Serial.print("WiFi Connected");

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

/*
    // Show WiFi status on LCD along with SSID of network
    lcd.setCursor(0, 0);
    lcd.print("   WiFi Connected   ");
    lcd.print(ssid);
*/
    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    BedroomLight = new Switch("Bedroom Light", 80, BedroomLightOn, BedroomLightOff);
    BedroomFan = new Switch("Bedroom Fan", 81, BedroomFanOn, BedroomFanOff);
    BedroomLamp = new Switch("Bedroom Lamp", 82, BedroomLampOn, BedroomLampOff);
    outletTwo = new Switch("Outlet Two", 83, outletTwoOn, outletTwoOff);

    //Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*BedroomLight);
    upnpBroadcastResponder.addDevice(*BedroomFan);
    upnpBroadcastResponder.addDevice(*BedroomLamp);
    upnpBroadcastResponder.addDevice(*outletTwo);

    //Set relay pins to outputs
    pinMode(16, OUTPUT);
    pinMode(05, OUTPUT);
    pinMode(04, OUTPUT);
    pinMode(00, OUTPUT);

  /*
    //Create Polling Message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Polling Status   ");
    lcd.setCursor(0, 2);
    lcd.print("  Of Smart Devices  ");
    delay(2000);
*/
    //Set each relay pin to HIGH this display status messagefor each relay
    digitalWrite(relayOne, HIGH);   // sets relayOne on
  /*  lcd.clear();
    lcd.print("Bedroom Light: Off     ");   */
    delay(500);
    digitalWrite(relayTwo, HIGH);   // sets relayOne on
   /* lcd.setCursor(0, 1);
    lcd.print("Bedroom Fan: Off     ");    */
    delay(500);
    digitalWrite(relayThree, HIGH);   // sets relayOne on
    /*lcd.setCursor(0, 2);
    lcd.print("Bedroom Lamp: Off     ");    */
    delay(500);
    digitalWrite(relayFour, HIGH);   // sets relayOne on
    delay(500);
  /*  lcd.setCursor(0, 3);
    lcd.print("Outlet Two: Off     ");  */

/*
    //Create system initialized message
    lcd.clear();
    lcd.setCursor(0, 0);
    delay(1000);
    lcd.print(" System Initialzed  ");
    delay(1000);
    lcd.setCursor(0, 2);
    lcd.print(" Ready For Commands ");
    delay(2000);

  
    //Set up device status message
    lcd.clear();
    lcd.print("Bedroom Light: Off     ");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("Bedroom Fan: Off     ");
    delay(500);
    lcd.setCursor(0, 2);
    lcd.print("Bedroom Lamp: Off     ");
    delay(500);
    lcd.setCursor(0, 3);
    lcd.print("Outlet Two: Off     ");
*/

  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();
    BedroomLight->serverLoop();
    BedroomFan->serverLoop();
    BedroomLamp->serverLoop();
    outletTwo->serverLoop();
  }
}

void BedroomLightOn() {
  // Serial.print("Switch 1 turn on ...");
  digitalWrite(relayOne, HIGH);   // sets relayOne on
 /* lcd.setCursor(0, 0);
  lcd.print("Bedroom Light: On      ");  */
}

void BedroomLightOff() {
  // Serial.print("Switch 1 turn off ...");
  digitalWrite(relayOne, LOW);   // sets relayOne off
 /* lcd.setCursor(0, 0);
  lcd.print("Bedroom Light: Off     ");    */
}

void BedroomFanOn() {
  // Serial.print("Switch 2 turn on ...");
  digitalWrite(relayThree, HIGH);   // sets relayTwo on
  /* lcd.setCursor(0, 1);
  lcd.print("Bedroom Fan: On      ");  */
}

void BedroomFanOff() {
  // Serial.print("Switch 2 turn off ...");
  digitalWrite(relayThree, LOW);   // sets relayTwo Off
 /* lcd.setCursor(0, 1);
  lcd.print("Bedroom Fan: Off     ");   */
}

//sockets

void BedroomLampOn() {
  //  Serial.print("Socket 1 turn on ...");
  digitalWrite(relayFour, HIGH);   // sets relayThree on
 /* lcd.setCursor(0, 2);
  lcd.print("Bedroom Lamp: On      "); */
}

void BedroomLampOff() {
  // Serial.print("Socket 1 turn off ...");
  digitalWrite(relayFour, LOW);   // sets relayThree off
 /* lcd.setCursor(0, 2);
  lcd.print("Bedroom Lamp: Off     ");   */
}

void outletTwoOn() {
  // Serial.print("Socket 2 turn on ...");
  digitalWrite(relayTwo, HIGH);   // sets relayFour on
/*  lcd.setCursor(0, 3);
  lcd.print("Outlet Two: On      ");    */
}

void outletTwoOff() {
  // Serial.print("Socket 2 turn off ...");
  digitalWrite(relayTwo, LOW);   // sets relayFour off
 /* lcd.setCursor(0, 3);
  lcd.print("Outlet Two: Off     ");     */
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  // Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
    //  Serial.println("");
    //  Serial.print("Connected to ");
    //  Serial.println(ssid);
    // Serial.print("IP address: ");
    //  Serial.println(WiFi.localIP());
  }
  else {
    // Serial.println("");
    //Serial.println("Connection failed.");
  }

  return state;
}
