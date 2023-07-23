#include <LiquidCrystal.h> 
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 
 
#include <SoftwareSerial.h> 
SoftwareSerial SIM900A(11,12); 
 
int gas_sensor = A0;  // choose the input pin (for Gas sensor)   
int buzzer = 13;      // choose the pin for the Buzzer 
int green_led = 8;    // choose the pin for the Green LED 
int red_led = 9;      // choose the pin for the Red Led 
int yellow_led = 10;  // choose the pin for the Yellow Led 
 
int read_value;       // variable (reading the gas pin status) status 
int set_low = 400;    // low exposure limit of LPG (ppm) 
int set_high = 600;   // high exposure limit of LPG (ppm) 
 
void setup() 
{  
  pinMode(gas_sensor, INPUT);  // declare sensor as input   
  pinMode(buzzer,OUTPUT);      // declare Buzzer as output    
  pinMode(red_led,OUTPUT);     // declare Red LED as output   
  pinMode(green_led,OUTPUT);   // declare Green LED as output   
  pinMode(yellow_led,OUTPUT);  // declare Yellow LED as output 
 
  randomSeed(analogRead(0)); 
 
  lcd.begin(16, 2);   
  lcd.clear(); 
 
  lcd.setCursor(0,0);   
  lcd.print("     iSense     "); 
  delay(2000); 
 
  lcd.setCursor(0,0);   
  lcd.print(" IOT Based Gas  ");   
  lcd.setCursor(0,1);   
  lcd.print("Leakage Detector"); 
  delay(2000); 
 
  lcd.setCursor(0,0);   
  lcd.print(" Targeted Gas:  ");   
  lcd.setCursor(0,1);   
  lcd.print("      LPG       "); 
  delay(2000); 
 
  lcd.clear(); 
 
  SIM900A.begin(9600);// Setting the baud rate of GSM Module   
  Serial.begin(9600); // baud rate of Serial Monitor (Arduino) 
  Serial.println ("SIM900A Ready"); 
  delay(100); 
}  

void loop() { 
  read_value = (analogRead(gas_sensor)); // read input value 
 
  read_value = read_value - 150; 
  if(read_value < 0) 
  { 
    read_value = 0; 
  }  
  lcd.setCursor(0, 0);   
  lcd.print("PPM Level: ");   
  lcd.print(read_value);   
  lcd.print("  "); 
 
  if(read_value > set_high) // high exposure 
  {   
    lcd.setCursor(0, 1);     
    lcd.print("DANGER! GAS LEAK");     
    digitalWrite(buzzer, HIGH);     // Turn Buzzer on     
    digitalWrite(red_led, HIGH);    // Turn LED on     
    digitalWrite(green_led, LOW);   // Turn LED off     
    digitalWrite(yellow_led, LOW);  // Turn LED off 
    delay(1000); 
 
    SendMessage(); // Send Message 
  } 
   
  if(read_value > set_low && read_value < set_high) // low exposure 
  {   
    lcd.setCursor(0, 1);     
    lcd.print("Possible Leaking");     
    digitalWrite(buzzer, LOW);      // Turn Buzzer off      
    digitalWrite(red_led, LOW);     // Turn LED off     
    digitalWrite(green_led, LOW);   // Turn LED off     
    digitalWrite(yellow_led, HIGH); // Turn LED on    
    delay(1000); 
  }  
  if(read_value < set_low) // normal exposure 
  {   
    lcd.setCursor(0, 1); 
    lcd.print("   No Leaking   ");    
    digitalWrite(buzzer, LOW);      // Turn Buzzer off       
    digitalWrite(red_led, LOW);     // Turn LED off     
    digitalWrite(green_led, HIGH);  // Turn LED on    
    digitalWrite(yellow_led, LOW);  // Turn LED off 
  }  
  delay(100); 
} 
  
void SendMessage() 
{ 
  Serial.println ("Sending Message"); 
  // Sets the GSM Module in Text Mode 
  SIM900A.println("AT+CMGF=1");     
  delay(1000); 
  Serial.println ("Set SMS Number"); 
  // Mobile phone number to send message 
  SIM900A.println("AT+CMGS=\"+60106540804\"\r");    delay(1000); 
  Serial.println ("Set SMS Content"); 
  // Messsage content 
  SIM900A.println("DANGER! GAS LEAKING!!!"); 
  delay(100); 
  Serial.println ("Finish"); 
  // ASCII code of CTRL+Z 
  SIM900A.println((char)26); 
  delay(1000); 
  Serial.println ("Message has been sent"); 
} 
 
