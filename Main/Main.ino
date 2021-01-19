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

//Max waardes Sensoren
int Max_Temperatuur_Sensor = 4000;
int Max_Grondvochtigheid_Sensor = 4000;
int Max_Ldr_Sensor = 4000;


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
    
    // Als sensor defect is(ERROR)
    if(Ldr_Sensor == 0){
      Serial.println("Error Ldr_Sensor Kapot"); 
    }
    
    if(Temperatuur_Sensor == 0){
      Serial.println("Error Temperatuur_Sensor Kapot"); 
    }
    
    if(Grondvochtigheid_Sensor == 0){
      Serial.println("Error Grondvochtigheid_Sensor Kapot"); 
    }
  }

  //Na opstart connectie met de API
  Ldr_Sensor = analogRead(Ldr_Pin);
  Grondvochtigheid_Sensor = analogRead(Grondvochtigeheid_Pin);
  Temperatuur_Sensor = analogRead(Temperatuur_Pin);

  //Foute waarde gemeten (ERROR)
  if(Ldr_Sensor > Max_Ldr_Sensor){
    Serial.println("Error Ldr_Sensor Foute waarde"); 
  }
  
  if(Temperatuur_Sensor > Max_Temperatuur_Sensor){
    Serial.println("Error Temperatuur_Sensor Foute waarde"); 
  }
  
  if(Grondvochtigheid_Sensor > Max_Grondvochtigheid_Sensor){
    Serial.println("Error Grondvochtigheid_Sensor Foute waarde"); 
  }
  
  


  //Debugging
//  Serial.println("Debugging");
//  Serial.println(Temperatuur_Sensor);
//  Serial.println(Grondvochtigheid_Sensor);
//  Serial.println(Ldr_Sensor);
//  Serial.println("End Debugging"); 
//  delay(1000);
}
