#include <SoftwareSerial.h> 
SoftwareSerial MyBlue(50, 51); // RX | TX 
int flag = 0; 
int LED = 13; 
void setup() 
{   
 Serial.begin(9600); 
 MyBlue.begin(9600); 
 pinMode(LED, OUTPUT); 
 Serial.println("Ready to connect\nDefualt password is 1234 or 000"); 
} 
void loop() 
{ 
 if (MyBlue.available()) 
   flag = MyBlue.read(); 
 if (flag == 49) 
 { 
   digitalWrite(LED, HIGH); 
   Serial.println("LED On"); 
 } 
 else 
 { 
   digitalWrite(LED, LOW); 
   Serial.println(flag); 
 } 
}  
