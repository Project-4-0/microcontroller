//Includes
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "RunningMedian.h"


#define TIME_TO_SLEEP 1         // tijd in seconden dat de sleep modus actief is

//Wifi credentials
const char* ssid = "Van Pelt Homehotspot";
const char* password = "Speedy2169";

//Your Domain name with URL path or IP address with path
const char* serverNameMeasurements = "https://vito-api-dev.herokuapp.com/measurements";
const char* serverNamemacAddress = "https://vito-api-dev.herokuapp.com/boxes/macAddress";
const char* serverNameAddSensorArduino = "https://vito-api-dev.herokuapp.com/boxes/add_sensor_arduino";

//Variabels
String MacAddress;
String Variable_Box_Id;
int Temperatuur_Sensor_Waarde = 0;

String Sensor_Id_LDR;
String Sensor_Id_Grondvochtigheid;

int sensorsdoorgestuurd =0;
String hhtpResponseAddSensorArduino = "400";

//PinOut
int Temperatuur_Pin = 36;
int Grondvochtigeheid_Pin = 39;
int Ldr_Pin = 34;


//initialisatie mediaan
RunningMedian samples_LDR = RunningMedian(5);
RunningMedian samples_Grondvochtigheid = RunningMedian(5);

long count_LDR = 0;
long count_Grondvochtigheid = 0;

int mediaan_LDR_Waarde = 0;
int mediaan_Grondvochtigheid_Waarde = 0;


//response server
String responseMacAddress;
String responseAddSensorArduino;

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
int postDataToServerMeasurments(String x,String y, int z) {
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

String postDataToServerMacAddress() {
  Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (WiFi.status()== WL_CONNECTED) {
    HTTPClient http;   
    http.begin(serverNamemacAddress);  
    http.addHeader("Content-Type", "application/json");         
    StaticJsonDocument<200> doc;
    // Add values in the document
    doc["MacAddress"] = MacAddress;


    String requestBody;
    serializeJson(doc, requestBody);
    int httpResponseCode = http.POST(requestBody);
    if(httpResponseCode>0){
      String response = http.getString();                       
      //Serial.println(httpResponseCode);   
      //Serial.println(response);
      responseMacAddress = response.substring(9,10);
    }
    else {
      Serial.printf("Error occurred while sending HTTP POST: %s\n"); 
    }
  }
  
}

String postDataToServerAddSensorArduinoLicht(String x, String y, String z) {
  Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (WiFi.status()== WL_CONNECTED) {
    HTTPClient http;   
    http.begin(serverNameAddSensorArduino);  
    http.addHeader("Content-Type", "application/json");         
    StaticJsonDocument<200> doc;
    // Add values in the document
    doc["BoxID"] = x;
    doc["SensorName"] = y;
    doc["SensorType"] = z;

    String requestBody;
    serializeJson(doc, requestBody);
    int httpResponseCode = http.POST(requestBody);
    if(httpResponseCode>0){
      String response = http.getString();                       
      Serial.println(httpResponseCode);   
      Serial.println(response);
      responseAddSensorArduino = response.substring(12,14);
      //Serial.println(responseAddSensorArduino);
      Sensor_Id_LDR = responseAddSensorArduino;

    }
    else {
      Serial.printf("Error occurred while sending HTTP POST: %s\n"); 
    }
  }
  
}

String postDataToServerAddSensorArduinoBodemvochtigheid(String x, String y, String z) {
  Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (WiFi.status()== WL_CONNECTED) {
    HTTPClient http;   
    http.begin(serverNameAddSensorArduino);  
    http.addHeader("Content-Type", "application/json");         
    StaticJsonDocument<200> doc;
    // Add values in the document
    doc["BoxID"] = x;
    doc["SensorName"] = y;
    doc["SensorType"] = z;

    String requestBody;
    serializeJson(doc, requestBody);
    int httpResponseCode = http.POST(requestBody);
    if(httpResponseCode>0){
      String response = http.getString();                       
      Serial.println(httpResponseCode);   
      Serial.println(response);
      responseAddSensorArduino = response.substring(12,14);
      //Serial.println(responseAddSensorArduino);
      Sensor_Id_Grondvochtigheid = responseAddSensorArduino;

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
    http.begin(serverNamemacAddress);  
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


//functie voor mediaan te bereken
void mediaan_LDR()
{
  if (count_LDR == 5){ 
    count_LDR = 0;
    mediaan_LDR_Waarde = samples_LDR.getMedian();
    samples_LDR.clear();
    }; 
  count_LDR++;

  long x = (analogRead(Ldr_Pin)/4095.0)*100;

  samples_LDR.add(x);

}

void mediaan_Grondvochtigheid()
{
  if (count_Grondvochtigheid == 5){ 
    count_Grondvochtigheid = 0;
    mediaan_Grondvochtigheid_Waarde = samples_Grondvochtigheid.getMedian();
    samples_Grondvochtigheid.clear();
    }; 
  count_Grondvochtigheid++;

  long x = (analogRead(Grondvochtigeheid_Pin)/4095.0)*100;

  samples_Grondvochtigheid.add(x);

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
  if(Variable_Box_Id == ""){
    //Request Variable_Box_Id API
    postDataToServerMacAddress();
    Variable_Box_Id = responseMacAddress; 
    Serial.println("Nieuwe Variable_Box_Id toegekend"); 
    //Serial.println(Variable_Box_Id);
  }

  if(Sensor_Id_LDR == ""){
  postDataToServerAddSensorArduinoLicht(Variable_Box_Id,"LDR","Licht");
  }
  if(Sensor_Id_Grondvochtigheid == ""){
  postDataToServerAddSensorArduinoBodemvochtigheid(Variable_Box_Id,"Bodemvochtigheid_B_one","Bodemvochtigheid");
  }
  //Na opstart connectie met de API
  for(int i=0; i<4; i++){
  mediaan_LDR();
  }
  for(int i=0; i<4; i++){
  mediaan_Grondvochtigheid();
  }
  
  Temperatuur_Sensor_Waarde = (analogRead(Temperatuur_Pin)/4095.0)*100;
  
  
  //Doorsturen van data naar de API
  //delay(2000);
  postDataToServerMeasurments(Variable_Box_Id,Sensor_Id_LDR,mediaan_LDR_Waarde);
  postDataToServerMeasurments(Variable_Box_Id,Sensor_Id_Grondvochtigheid,mediaan_Grondvochtigheid_Waarde);
  
  //getDataFromServer();
  delay(2000);
  

  //Start de Slaap modus
  //Go_To_Sleep();



}
