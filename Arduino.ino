#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "esp32-hal-ledc.h"
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPI.h>
#include <Ethernet.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#define LED_PIN   15 // ESP32 pin GIOP15 connected to LED's pin
#define SOUND_SPEED 0.034
#define DISTANCE_THRESHOLD 50 // centimeters
#define COUNT_LOW 0
#define COUNT_HIGH 8888
#define TIMER_WIDTH 16

// our tank height is 200cm

// The below are variables, which can be changed

const int trigPin = 5;
const int echoPin = 18;
long duration;
float distanceCm;
float tank_level;
 

WiFiMulti wifiMulti;

char server[] = "172.16.4.123"; // local IP address
IPAddress ip(172,16,4,123); 
WiFiClient client; 

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
 

const char* ssid = "Free_Wife";
const char* password = "Hoseah18#";

//int SERVO_PIN = 13;
//Servo servo;

void setup() {
  Serial.begin (115200);       // initialize serial port
  pinMode(trigPin, OUTPUT); // set ESP32 pin to output mode
  pinMode(echoPin, INPUT);  // set ESP32 pin to input mode
  pinMode(LED_PIN, OUTPUT);  // set ESP32 pin to output mode
  ledcSetup(1, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
  ledcAttachPin(2, 1);   // GPIO 22 assigned to channel 1

  lcd.init();  //initialize the LCD
  lcd.clear(); 
   
  //Print a message to the LCD
  lcd.backlight();
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tracking system");
  lcd.setCursor(2,1);
  lcd.print("Water level");
  delay(500);
   
  //servo.attach(SERVO_PIN);   // attaches the servo on pin 9 to the servo object
  //servo.write(angle);

 wifiMulti.addAP("Free_Wife", "12345678900");
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  tank_level= 200-distanceCm;
  
  if (distanceCm > 14.99 && distanceCm <99.99){
    for (int i=COUNT_LOW ; i < COUNT_HIGH ; i=i+100) {
    ledcWrite(1, i);       // sweep servo 1
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    }

  }

  if (distanceCm > 14.99 && distanceCm <99.99){
    digitalWrite(LED_PIN, HIGH);
    
  }else {
    digitalWrite(LED_PIN, LOW);
  }

     
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
   
  lcd.clear();
  lcd.print("Duration: ");
  lcd.print(duration);
  lcd.setCursor(0,1);
  lcd.print("Distance: ");
  lcd.print(distanceCm);  
  delay(500);

  Sending_To_phpmyadmindatabase(); 
  delay(10000); // interval
}
 void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
 {
   if((wifiMulti.run() == WL_CONNECTED)) {
    
   Serial.print("[HTTP] begin...\n");
   if (client.connect(server, 80)) {
    Serial.println("connected");

   
   Serial.print("GET /water_level/insert.php?tank_level=");
    client.print("GET /water_level/insert.php?tank_level=");     //YOUR URL
    Serial.println(tank_level);
    client.print(tank_level);
   // client.print("&temperature=");
    //Serial.println("&temperature=");
    //client.print(temperature );
    //Serial.println(temperature );
   
   //client.print("&groupName=Kanla");

   //Serial.print("GET /water_level/insert.php?id=1&tank_location=fabrice");
   //client.print("GET /water_level/insert.php?id=1&tank_location=fabrice");
   
    //Serial.println("&groupname=Kanla");
   // client.print(temperatureData);
    //Serial.println(temperatureData);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host:172.16.4.123");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }else {
      Serial.printf("[HTTP} Unable to connect\n");
   }
   }
