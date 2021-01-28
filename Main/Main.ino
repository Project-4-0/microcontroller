//Includes
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define TIME_TO_SLEEP 5         // tijd in seconden dat de sleep modus actief is

//Wifi credentials
const char* ssid = "Van Pelt Homehotspot";
const char* password = "Speedy2169";

//Your Domain name with URL path or IP address with path
const char* serverNameMeasurements = "https://vito-api-dev.herokuapp.com/measurements";
const char* serverName = "https://vito-youri-api.herokuapp.com/test";


//Variabels
String MacAddress;
int Variable_Box_Id = 0;
int Temperatuur_Sensor_Waarde = 0;
int Grondvochtigheid_Sensor_Waarde = 0;
int Ldr_Sensor_Waarde = 0;

int Sensor_Id_LDR = 1;

//PinOut
int Temperatuur_Pin = 36;
int Grondvochtigeheid_Pin = 39;
int Ldr_Pin = 34;

//Max waardes Sensoren
int Max_Temperatuur_Sensor = 95;
int Max_Grondvochtigheid_Sensor = 95;
int Max_Ldr_Sensor = 95;


//Functie voor de slaap te activeren
void Go_To_Sleep(){
  Serial.println("Going to sleep");
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 1000000);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,1);
  delay(200);
  esp_light_sleep_start();
}

//Functie voor data naar API te sturen
int postDataToServer(int x,int y, int z) {
  Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (WiFi.status()== WL_CONNECTED) {
    HTTPClient http;   
    http.begin(serverNameMeasurements);  
    http.addHeader("Content-Type", "application/json");         
    StaticJsonDocument<200> doc;
    // Add values in the document
    doc["BoxID"] = x;
    doc["SensorID"] = y;
    doc["Value"] = z;

    String requestBody;
    serializeJson(doc, requestBody);
    int httpResponseCode = http.POST(requestBody);
    if(httpResponseCode>0){
      String response = http.getString();                       
      Serial.println(httpResponseCode);   
      Serial.println(response);
    }
    else {
      Serial.printf("Error occurred while sending HTTP POST: %s\n"); 
    }
  }
}

void getDataFromServer(){
  Serial.println("Getting JSON data from server...");
  // Block until we are able to connect to the WiFi access point
  if (WiFi.status()== WL_CONNECTED) {
    HTTPClient http;   
    http.begin(serverName);  
     int httpCode = http.GET();        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
}
}

void setup(){
  Serial.begin(115200); // starts the serial port at 9600
  delay(100);
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  MacAddress = WiFi.macAddress();
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

 
void loop()
{
  //Eerste Keer Startup
  if(Variable_Box_Id == 0){
    //Request Variable_Box_Id API
    Variable_Box_Id = 1; 
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
Ldr_Sensor_Waarde = (analogRead(Ldr_Pin)/4095.0)*100;
Grondvochtigheid_Sensor_Waarde = (analogRead(Grondvochtigeheid_Pin)/4095.0)*100;
Temperatuur_Sensor_Waarde = (analogRead(Temperatuur_Pin)/4095.0)*100;


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
delay(2000);
postDataToServer(Variable_Box_Id,Sensor_Id_LDR,Ldr_Sensor_Waarde);
//postDataToServer(Variable_Box_Id,2,Grondvochtigheid_Sensor_Waarde);

//getDataFromServer();
delay(2000);

  //Debugging
//  Serial.println("Debugging");
//  Serial.println(Temperatuur_Sensor_Waarde);
//  Serial.println(Grondvochtigheid_Sensor_Waarde);
//  Serial.println(Ldr_Sensor_Waarde);
//  Serial.println("End Debugging"); 
//  delay(1000);


//Start de Slaap modus
Go_To_Sleep();



}
