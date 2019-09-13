#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include<SoftwareSerial.h> 
#include<Arduino.h>

#define LED_BUILTIN 2

SoftwareSerial myserial(3,1);
const char* ssid = "Holi:3";
const char* password = "amfj46571";

int estado = 1;
std::string data_;
std::string data_pantalla;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  myserial.begin(115200);//Serial connection
  Serial.begin(9600);
  WiFi.begin(ssid,password);   //WiFi connection
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    //Serial.print(WiFi.status() +" - ");
    delay(500);
    Serial.println("Waiting for connection");
  }
}


void loop() {

  
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

    if(Serial.available() >0){
      data_pantalla = Serial.read();
      
      if(data_pantalla == "&"){
        Serial.println(data_pantalla.c_str());  
        if(estado ==0){
          estado = 1;
        }else if(estado == 1){
          estado = 0;
          Serial.println(data_.c_str());
          POSTEAR(data_);
          data_ = "";
        }
      }else{
        data_ += data_pantalla;
      }
      //Serial.println("datos");
      //Serial.print("Se recibio data ");
      //Serial.println(data_pantalla.c_str());
    }else{
      //Serial.println(".");
      GET();  
    }
    
   
  }
}

void POSTEAR(std::string cadena){
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://192.168.43.240:8000/servidorApp/getStringLiquid");      //Specify request destination
    http.addHeader("Content-Type", "text/plain");  //Specify content-type header

    char char_array[cadena.size()+1];
    strcpy(char_array,cadena.c_str());
    int httpCode = http.POST(char_array);   //Send the request
    String payload = http.getString();                  //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection

/*   http.begin("http://192.168.1.5:3000/insertarDatos/9.1/9.1/1/9.1/puntouno/kb9/-9.2/-9.2");      //Specify request destination
   //https://api.appery.io/rest/1/apiexpress/api/HolaCulo?apiKey=36cec509-b5e8-4886-b777-f66aa12e5e3d
   //http.begin("https://api.appery.io/rest/1/apiexpress/api/HolaCulo?apiKey=36cec509-b5e8-4886-b777-f66aa12e5e3d");
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
   StaticJsonDocument<300> doc;
   doc["codigo"]=7;
   doc["temperatura"]=7777.7;
   doc["gas"]=7777.7;
   doc["lluvia"]=0;
   doc["peso"]=7777.7;
   doc["fecha"]="modulowifi";
   doc["hora"]="ahorita";
   doc["latitud"]=77.77;
   doc["longitud"]=77.77;
   Serial.println("LLEGA EL DOC QUE VOY A IMPRIMIR--> ");
   //serializeJsonPretty(doc,Serial);
   String finalisima;
   //serializeJson(doc,finalisima);
   Serial.println("LLEGA EL STRING QUE GUARDE CON LA INFO DEL JSON ARMADO-->");
   //Serial.println(finalisima);
   String xd="palabra: mertens";
   //Serial.println(xd);
   int httpCode = http.POST(xd);
////CODIGO VIEJITO DEL POST DE UN SIMPLE STRING
   String payload = http.getString();                  //Get the response payload
   Serial.println("ESTO VIENE DE RESPUESTA-->");
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
//
   http.end();  //Close connection
   delay(1000000000000);
//TERMINA CODIGO VIEJITO*/

}


void GET(){

//
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://192.168.43.240:8000/servidorApp/pruebaHola");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request

    if (httpCode > 0) { //Check the returning code

      String payload = http.getString();   //Get the request response payload
      Serial.println("payload get");
      Serial.println(payload);                     //Print the response payload
      myserial.print(payload);

    }else{
      Serial.println("httpCode recieved "+ httpCode);
    }

    http.end();   //Close connection

  }

  delay(30000);    //Send a request every 30 seconds
}
