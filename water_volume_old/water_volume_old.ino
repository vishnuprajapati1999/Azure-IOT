#include <ArduinoJson.h>
#include <ArduinoJson.hpp>


#include <FirebaseArduino.h> 
#include <ESP8266WiFi.h>    
#include <ArduinoJson.h>
#define WIFI_SSID "AndroidAP7B35"             //replace SSID with your wifi username
#define WIFI_PASSWORD "vishnu@1999"  
#define solenoid_valve D4
#define sensorPin D2
#define FIREBASE_HOST "panivala-d3e75.firebaseio.com"                         //link of api
#define FIREBASE_AUTH "v7gHvpqKWoBMw90no4P1osrhk0pVQDX76NSSMjYV"           //database secret

int sensorInterrupt = 0;  // interrupt 0
//int solenoidValve = 5; // Digital pin 5
//unsigned int SetPoint = 4000; //400 milileter
float vol,l_minute;

unsigned long currentTime;
unsigned long cloopTime;
/*The hall-effect flow sensor outputs pulses per second per litre/minute of flow.*/
float calibrationFactor = 4.5; //You can change according to your datasheet
void ICACHE_RAM_ATTR pulseCounter();
volatile byte pulseCount =0;  

float flowRate = 0.0;
unsigned int flowMilliLitres =0;
unsigned long totalMilliLitres = 0;
float FirebaseResult_copy;
unsigned long oldTime = 0;
int FirebaseResult;
ICACHE_RAM_ATTR void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
void setup()
{

  // Initialize a serial connection for reporting values to the host
  Serial.begin(115200);
  pinMode(solenoid_valve ,OUTPUT);
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  digitalWrite(solenoid_valve ,HIGH);
  attachInterrupt(digitalPinToInterrupt(sensorPin), pulseCounter, RISING); 
  currentTime = millis();
  cloopTime = currentTime;
  /*The Hall-effect sensor is connected to pin 2 which uses interrupt 0. Configured to trigger on a FALLING state change (transition from HIGH
  (state to LOW state)*/
  //attachInterrupt(sensorInterrupt, pulseCounter, FALLING); //you can use Rising or Falling
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  
  
  //connect to wifi
  while (WiFi.status() != WL_CONNECTED) {           //wait till connected to WiFi
      
                         //Blink the light till connected to WiFi
    delay(100);
    
    Serial.print("Wifi is not connected"); }
    
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);             //connect to Database
  delay(1000);
  if(Firebase.failed()) { 
     Serial.print("firebase failed:"); 
     Serial.println(Firebase.error());   
     return; 
 }

}

int firebaseGet(String s) {
  int x = (int)Firebase.getInt(s);
  yield();
  delay(100);
  return x;
  delay(100);
}
float firebaseGet_F(String s) {
  float x = (float)Firebase.getFloat(s);
  yield();
  delay(100);
  return x;
  delay(100);
}
void loop()
{

         
   // if (totalMilliLitres > 4000)
    //{
     // String firebaseResult = firebaseGet("led/status");
  //delay(100);
  //if (firebaseResult=="OFF"){
      //code to happen if the status is ON  
      //digitalWrite(WIFI_LED,HIGH);
    //   digitalWrite(solenoid_valve,HIGH);
      // Serial.print("ON");
  //}
    //}
    //else{
      //code to happen if the status is OFF
      //digitalWrite(WIFI_LED,LOW);
      //SetSolinoidValveoff();
      //digitalWrite(solenoid_valve,LOW);
      //Serial.print("OFF");
      
    //}
 //Reset the pulse counter so we can start incrementing again
    //pulseCount = 0;

    // Enable the interrupt again now that we've finished sending output
    //attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  //}
  

currentTime = millis();
   // Every second, calculate and print litres/hour
   if((millis() - oldTime) > 1000)
   { 
    int firebaseResult = firebaseGet("water");
  delay(100);
  float firebaseResult_copy = firebaseGet_F("water_copy");
  delay(100);
     cloopTime = currentTime; // Updates cloopTime
   
  if(firebaseResult_copy==0)
  {
  if (firebaseResult!=0){
     // code to happen if the status is ON  
      //digitalWrite(WIFI_LED,HIGH);
     digitalWrite(solenoid_valve,LOW);
       Serial.print("ON");
       if(pulseCount != 0){
        float firebaseResult_copy = firebaseGet_F("water_copy");
  
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (pulseCount / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      
      Serial.print(" L/M");
      l_minute = l_minute/60;
     vol=FirebaseResult_copy;
      vol = vol +l_minute;
      Serial.print("Vol:");
      Serial.print(vol);
      Serial.print(" L");
      FirebaseResult_copy=vol;
     
      Firebase.setFloat("water_copy", FirebaseResult_copy);
       pulseCount=0;
      Serial.print(l_minute, DEC); // Print litres/hour
      Serial.println(" L/Sec");
      
   
      if(firebaseResult==(int)FirebaseResult_copy)
      {
        digitalWrite(solenoid_valve,HIGH);
       firebaseResult=0;
       vol=0;
       FirebaseResult_copy=0; 
       
       Firebase.setInt("water", firebaseResult);
       Firebase.setFloat("water_copy", FirebaseResult_copy);
      }
      //flow_frequency = 0; // Reset Counter
      
}
else {
    Serial.println(" flow rate = 0 ");
}
  //}
    //}
    //
  
   }
   else{
      //code to happen if the status is OFF
      //digitalWrite(WIFI_LED,LOW);
      //SetSolinoidValveoff();
      digitalWrite(solenoid_valve,HIGH);
      Serial.print("OFF");
      
    }
  }
  else{
    
     // code to happen if the stMatus is ON  
      //digitalWrite(WIFI_LED,HIGH);
     digitalWrite(solenoid_valve,LOW);
       Serial.print("ON2");
       if(pulseCount != 0){
        float firebaseResult_copy = firebaseGet_F("water_copy");
    vol=FirebaseResult_copy;
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (pulseCount / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      
      Serial.print(" L/M");
      l_minute = l_minute/60;
     
      vol = vol +l_minute;
      Serial.print("Vol:");
      Serial.print(vol);
      Serial.print(" L");
      
      FirebaseResult=vol;
      
      Firebase.setFloat("water_copy", FirebaseResult_copy);
      pulseCount=0;
      Serial.print(l_minute, DEC); // Print litres/hour
      Serial.println(" L/Sec");
      
   
      if(firebaseResult==(int)FirebaseResult_copy)
      {
        digitalWrite(solenoid_valve,HIGH);
       firebaseResult=0;
       vol=0;
       FirebaseResult_copy=0; 
       
       Firebase.setInt("water", firebaseResult);
       Firebase.setFloat("water_copy", FirebaseResult_copy);
      }
    }
    else{
      Serial.print("flow_rate2=0");
      }
    
//Insterrupt Service Routine


}
   }
}
//void SetSolinoidValve()
//{
//  digitalWrite(solenoidValve, LOW);
//}
//void SetSolinoidValveoff()
//{
//  digitalWrite(solenoidValve, HIGH);
//}
