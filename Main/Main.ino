//Includes
#include <WiFi.h>

//Variabels



void setup()
{
  Serial.begin(9600); // starts the serial port at 9600
  delay(100);
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop()
{

}
