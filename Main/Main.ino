//Includes
#include <WiFi.h>

#define TIME_TO_SLEEP 1         // tijd in seconden dat de sleep modus actief is

//Variabels
String MacAddress;
int Variable_Box_Id = 0;
int Temperatuur_Sensor_Waarde = 0;
int Grondvochtigheid_Sensor_Waarde = 0;
int Ldr_Sensor_Waarde = 0;

//PinOut
int Temperatuur_Pin = 36;
int Grondvochtigeheid_Pin = 39;
int Ldr_Pin = 34;

//Max waardes Sensoren
int Max_Temperatuur_Sensor = 4000;
int Max_Grondvochtigheid_Sensor = 4000;
int Max_Ldr_Sensor = 4000;


//Functie voor de slaap te activeren
void Go_To_Sleep(){
  Serial.println("Going to sleep");
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 1000000);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,1);
  delay(200);
  esp_light_sleep_start();
}


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
    Ldr_Sensor_Waarde = analogRead(Ldr_Pin);
    Grondvochtigheid_Sensor_Waarde = analogRead(Grondvochtigeheid_Pin);
    Temperatuur_Sensor_Waarde = analogRead(Temperatuur_Pin);
    
    // Als sensor defect is(ERROR)
    if(Ldr_Sensor_Waarde == 0){
      Serial.println("Error Ldr_Sensor Kapot"); 
    }
    
    if(Temperatuur_Sensor_Waarde == 0){
      Serial.println("Error Temperatuur_Sensor Kapot"); 
    }
    
    if(Grondvochtigheid_Sensor_Waarde == 0){
      Serial.println("Error Grondvochtigheid_Sensor Kapot"); 
    }
  }

//Na opstart connectie met de API
Ldr_Sensor_Waarde = analogRead(Ldr_Pin);
Grondvochtigheid_Sensor_Waarde = analogRead(Grondvochtigeheid_Pin);
Temperatuur_Sensor_Waarde = analogRead(Temperatuur_Pin);

//Foute waarde gemeten (ERROR)
if(Ldr_Sensor_Waarde > Max_Ldr_Sensor){
  Serial.println("Error Ldr_Sensor Foute waarde"); 
}

if(Temperatuur_Sensor_Waarde > Max_Temperatuur_Sensor){
  Serial.println("Error Temperatuur_Sensor Foute waarde"); 
}

if(Grondvochtigheid_Sensor_Waarde > Max_Grondvochtigheid_Sensor){
  Serial.println("Error Grondvochtigheid_Sensor Foute waarde"); 
}

//Doorsturen van data naar de API



//Start de Slaap modus
delay(1000);
Go_To_Sleep();


  //Debugging
  Serial.println("Debugging");
  Serial.println(Temperatuur_Sensor_Waarde);
  Serial.println(Grondvochtigheid_Sensor_Waarde);
  Serial.println(Ldr_Sensor_Waarde);
  Serial.println("End Debugging"); 
  delay(1000);
}
