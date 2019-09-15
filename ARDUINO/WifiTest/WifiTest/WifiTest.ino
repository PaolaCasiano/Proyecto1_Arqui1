#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include<SoftwareSerial.h> 
#include<Arduino.h>

#define LED_BUILTIN 2
//#SoftwareSerial myserial(3,1);
SoftwareSerial myserial(0,1);
const char* ssid = "Holi:3";
const char* password = "amfj46571";

int estado = 1;
std::string data_;
std::string data_pantalla;
String datos;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  myserial.begin(9600);//Serial connection
  Serial.begin(9600);
  WiFi.begin(ssid,password);   //WiFi connection
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
}


void loop() {

  
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

    if(Serial.available() >0){
      datos = Serial.readString();
      POSTEAR(datos);
      delay(1);
      
    }else{
      GET();  
      delay(100);
      
    }
    
   
  }
}

void POSTEAR(String cadena){
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://192.168.43.240:8000/servidorApp/getStringLiquid");      //Specify request destination
    http.addHeader("Content-Type", "text/plain");  //Specify content-type header
    char char_array[cadena.length()+1];
    strcpy(char_array,cadena.c_str());
    int httpCode = http.POST(char_array);   //Send the request
    String payload = http.getString();                  //Get the response payload
 
    http.end();  //Close connection
    delay(1000);

}


void GET(){

//
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://192.168.43.240:8000/servidorApp/request_impresion");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request

    if (httpCode > 0) { //Check the returning code

      String payload = http.getString();   //Get the request response payload
      if(payload !="E" && payload !="F" && payload != "H"&&  payload != ""){
        Serial.println( http.getString()+"&");                     //Print the response payload
        //delay(1000);
      }
      //Serial.println("se imprimio payload");  

    }else{
      Serial.println("httpCode recieved "+ httpCode);
    }

    http.end();   //Close connection

  }

}
