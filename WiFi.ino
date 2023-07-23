#define BLYNK_TEMPLATE_ID "*****" 
#define BLYNK_TEMPLATE_NAME "*****" 
#define BLYNK_AUTH_TOKEN "***** 
 
#define BLYNK_PRINT Serial 
 
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h> 
 
// WiFi credentials. 
char ssid[] = "*****"; 
char pass[] = "*****"; 
 
BlynkTimer timer; 
 
int sensorValue = 0; 
int green_led = D0;   // choose the pin for the Green LED 
int red_led = D1;     // choose the pin for the Red LED 
int yellow_led = D2;  // choose the pin for the Yellow LED 
 
BLYNK_CONNECTED() 
{ 
  Blynk.syncVirtual(V0); 
  Blynk.syncVirtual(V1); 
  Blynk.syncVirtual(V2); 
  Blynk.syncVirtual(V3); 
}  

void sensorDataSend() 
{ 
  sensorValue = analogRead(A0); // read input value 
   
  sensorValue = sensorValue - 300; 
  if(sensorValue < 0) 
  { 
    sensorValue = 0; 
  } 
  //Serial.println(sensorValue);      
  Blynk.virtualWrite(V0, sensorValue);  // send value to Blynk  
}  

void updateLED1() {   
    if (digitalRead(D0) == HIGH) {     
        Blynk.virtualWrite(V1, 1); 
        // Notification 
        Blynk.logEvent("no_leaking", "Normal. No Leaking Detected"); }   
    else { 
        Blynk.virtualWrite(V1, 0); } 
}  

void updateLED2() {   
    if (digitalRead(D1) == HIGH) {    
         Blynk.virtualWrite(V2, 1); 
        // Notification 
        Blynk.logEvent("gas_leaking", "DANGER! GAS LEAKING!"); }    
    else { 
        Blynk.virtualWrite(V2, 0); } 
}  

void updateLED3() {   
    if (digitalRead(D2) == HIGH) {     
        Blynk.virtualWrite(V3, 1); 
        // Notification 
        Blynk.logEvent("possible_leaking", "Beware! Possible Leaking Detected"); }    
    else { 
        Blynk.virtualWrite(V3, 0); } 
}  

void setup() 
{ 
  pinMode(D0, INPUT);    
  pinMode(D1, INPUT);   
  pinMode(D2, INPUT); 
 
  Serial.begin(9600); 
 
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); 
 
  timer.setInterval(10L, sensorDataSend);    
  timer.setInterval(1L, updateLED1);   
  timer.setInterval(1L, updateLED2);    
  timer.setInterval(1L, updateLED3);  
}  

void loop() 
{ 
  Blynk.run();   
  timer.run();
}
