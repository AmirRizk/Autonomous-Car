//Created by engineers: 
//Youwakim Rizk Youwakim - Email :amirrizkamirrizk4@outlook.com
//Abdallah Ebraheem Abdallah - Email : bodda88hema@gmail.com
//Project name : - Autonomous Car
//Group:- Emb 519 / 517
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ENA   16           // Enable/speed motors Right        GPIO14(D5)
#define ENB   0            // Enable/speed motors Left         GPIO12(D6)
#define ENC   12           // Enable/speed motors Left         GPIO12(D6)
#define IN_1  5            // L298N in1 motors Right           GPIO15(D8)
#define IN_2  4            // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2            // L298N in3 motors Left            GPIO2(D4)
#define IN_4  14           // L298N in4 motors Left            GPIO0(D3)
#define IN_5  13           // L298N in3 motors Left            GPIO2(D4)
#define IN_6  15           // L298N in4 motors Left            GPIO0(D3)
#define SOUND_VELOCITY 0.034
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Servo.h> // servo library  
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <HCSR04.h>
#include <NewPing.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String command; //String to store app command state.
int pos=0;
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;
long duration;
float distanceCm;
const int trigPin = 13;
const int echoPin = 15;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* ssid = "NodeMCU Car";
ESP8266WebServer server(80);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Servo s1;  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
 s1.attach(10);  // servo attach SD3 pin of ESP8266   

 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);
 pinMode(ENC, OUTPUT);  
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
 pinMode(IN_5, OUTPUT);
 pinMode(IN_6, OUTPUT); 
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
 pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  //GPIO 1 (TX) swap the pin to a GPIO.
 pinMode(1, FUNCTION_3);
 //GPIO 3 (RX) swap the pin to a GPIO.
 pinMode(3, FUNCTION_3);
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  Serial.begin(115200);
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
// Connecting WiFi

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int scan(){
  // Clears the trigPin
  if (pos < 180) {
       pos+=10; 
   s1.write(pos);
}
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  return distanceCm;
   delay(100);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void car() {
int distance_0;
distance_0 = scan();
  // Keep moving forward in a straight line while distance of objects in front > 15cm away
  while(distance_0 > 15)
  {
     goAhead();
     distance_0 = scan();
  }
}
// Go back and turn slightly right to move car in new direction
// This function only runs if an obstacle within 15cm is detected
void avoid()
{
    goBack();
    goRight();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void goAhead(){ 
   if (distanceCm>=200)
{
  
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);

      digitalWrite(IN_5, LOW);
      digitalWrite(IN_6, HIGH);
      analogWrite(ENC, speedCar);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void goBack(){
 
 if (distanceCm>=200)
{
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);

      digitalWrite(IN_5, HIGH);
      digitalWrite(IN_6, LOW);
      analogWrite(ENC, speedCar);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void goRight(){  

  if (distanceCm>=200){
{
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);
      
      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);

      digitalWrite(IN_5, HIGH);
      digitalWrite(IN_6, LOW);
      analogWrite(ENC, speedCar);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void goLeft(){

if (distanceCm>=200)
{
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);

      digitalWrite(IN_5, LOW);
      digitalWrite(IN_6, HIGH);
      analogWrite(ENC, speedCar);

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

 }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void goAheadRight(){
    
      if (distanceCm>=200)
{
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar/speed_Coeff);
 
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);

    
   }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void goAheadLeft(){

      if (distanceCm>=200)
{
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar/speed_Coeff);
      
      digitalWrite(IN_5, LOW);
      digitalWrite(IN_6, HIGH);
      analogWrite(ENB, speedCar/speed_Coeff);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void goBackRight(){ 

if (distanceCm>=200)
{
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar/speed_Coeff);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
      
      digitalWrite(IN_5, HIGH);
      digitalWrite(IN_6, LOW);
      analogWrite(ENC, speedCar);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void goBackLeft(){ 

if (distanceCm>=200)
{
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar/speed_Coeff);
      
      digitalWrite(IN_5, HIGH);
      digitalWrite(IN_6, LOW);
      analogWrite(ENB, speedCar/speed_Coeff);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void stopCar(){  
if (distanceCm<200)
{
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
      
      digitalWrite(IN_5, LOW);
      digitalWrite(IN_6, LOW);
      analogWrite(ENC, speedCar);
 }
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
      
      digitalWrite(IN_5, LOW);
      digitalWrite(IN_6, LOW);
      analogWrite(ENC, speedCar);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
    server.handleClient();
    
      command = server.arg("State");
      if      (command == "F") goAhead();
      else if (command == "B") goBack();
      else if (command == "L") goLeft();
      else if (command == "R") goRight();
      else if (command == "I") goAheadRight();
      else if (command == "G") goAheadLeft();
      else if (command == "J") goBackRight();
      else if (command == "H") goBackLeft();
      else if (command == "0") speedCar = 400;
      else if (command == "1") speedCar = 470;
      else if (command == "2") speedCar = 540;
      else if (command == "3") speedCar = 610;
      else if (command == "4") speedCar = 680;
      else if (command == "5") speedCar = 750;
      else if (command == "6") speedCar = 820;
      else if (command == "7") speedCar = 890;
      else if (command == "8") speedCar = 960;
      else if (command == "9") speedCar = 1023;
      else if (command == "S") stopCar(),scan();

     
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
