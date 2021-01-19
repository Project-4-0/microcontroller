//Includes
#include <WiFi.h>

//Variabels
String MacAddress;
int Variable_Box_Id = 0;
int Temperatuur_Sensor = 0;
int Grondvochtigheid_Sensor = 0;
int Ldr_Sensor = 0;

//PinOut
int Temperatuur_Pin = 36;
int Grondvochtigeheid_Pin = 39;
int Ldr_Pin = 34;


void setup()
{
  Serial.begin(9600); // starts the serial port at 9600
  delay(100);
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  MacAddress = WiFi.macAddress();
}
 
void loop()
{
  //Eerste Keer Startup
  if(Variable_Box_Id == 0){
    //Request Variable_Box_Id API
    Variable_Box_Id = 15; 
    Serial.println("Nieuwe Variable_Box_Id toegekend");  
    //Testen Meting Sensoren
    Ldr_Sensor = analogRead(Ldr_Pin);
    Grondvochtigheid_Sensor = analogRead(Grondvochtigeheid_Pin);
    Temperatuur_Sensor = analogRead(Temperatuur_Pin);
    // Als er fout is Print fout
    if(Ldr_Sensor == 0){
      Serial.println("Error Ldr_Sensor"); 
    }
    if(Temperatuur_Sensor == 0){
      Serial.println("Error Temperatuur_Sensor"); 
    }
    if(Grondvochtigheid_Sensor == 0){
      Serial.println("Error Grondvochtigheid_Sensor"); 
    }

  }
  Serial.println("geslaagd");
  Serial.println(Temperatuur_Sensor);
  Serial.println(Grondvochtigheid_Sensor);
  Serial.println(Ldr_Sensor); 
  delay(1000);
}
