#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define LED_BUILTIN 2

// const char* ssid = "TURBONETT-CJ";
//const char* password = "3F940C0426";
//
//void setup () {
//
//Serial.begin(115200);
//WiFi.begin(ssid, password);
//
//while (WiFi.status() != WL_CONNECTED) {
//
//delay(1000);
//Serial.print("Connecting..");
//
//}
//
//}
//
//void loop() {
//
//if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
//
//HTTPClient http;  //Declare an object of class HTTPClient
//
//http.begin("http://192.168.1.8:3000/ultimaPalabra");  //Specify request destination
//int httpCode = http.GET();                                                                  //Send the request
//
//if (httpCode > 0) { //Check the returning code
//
//String payload = http.getString();   //Get the request response payload
//Serial.println(payload);                     //Print the response payload
//
//}
//
//http.end();   //Close connection
//
//}
//
//delay(30000);    //Send a request every 30 seconds
//
//}
//AQUI SEPARO EL GET DEL POST, EL GET ESTA ARRIBA Y EL POST DE AQUI EN ADELANTE---------------------------------------------------------------------------------------------------------------
const char* ssid = "Holi:3";
const char* password = "amfj46571";
//
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
//
  Serial.begin(115200);                                  //Serial connection
  WiFi.begin(ssid,password);   //WiFi connection
//
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
//
    delay(500);
    Serial.println("Waiting for connection");
//
  }
//
}
//
void loop() {
//
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);  
  return;
//
//    HTTPClient http;    //Declare object of class HTTPClient
// //

//    http.begin("http://192.168.1.5:3000/insertarDatos/9.1/9.1/1/9.1/puntouno/kb9/-9.2/-9.2");      //Specify request destination
//    //https://api.appery.io/rest/1/apiexpress/api/HolaCulo?apiKey=36cec509-b5e8-4886-b777-f66aa12e5e3d
//    //http.begin("https://api.appery.io/rest/1/apiexpress/api/HolaCulo?apiKey=36cec509-b5e8-4886-b777-f66aa12e5e3d");
//    http.addHeader("Content-Type", "text/plain");  //Specify content-type header
//    StaticJsonDocument<300> doc;
//    doc["codigo"]=7;
//    doc["temperatura"]=7777.7;
//    doc["gas"]=7777.7;
//    doc["lluvia"]=0;
//    doc["peso"]=7777.7;
//    doc["fecha"]="modulowifi";
//    doc["hora"]="ahorita";
//    doc["latitud"]=77.77;
//    doc["longitud"]=77.77;
//    Serial.println("LLEGA EL DOC QUE VOY A IMPRIMIR--> ");
//    //serializeJsonPretty(doc,Serial);
//    String finalisima;
//    //serializeJson(doc,finalisima);
//    Serial.println("LLEGA EL STRING QUE GUARDE CON LA INFO DEL JSON ARMADO-->");
//    //Serial.println(finalisima);
//    String xd="palabra: mertens";
//    //Serial.println(xd);
//    int httpCode = http.POST(xd);
// ////CODIGO VIEJITO DEL POST DE UN SIMPLE STRING
//    String payload = http.getString();                  //Get the response payload
//    Serial.println("ESTO VIENE DE RESPUESTA-->");
//    Serial.println(httpCode);   //Print HTTP return code
//    Serial.println(payload);    //Print request response payload
// //
//    http.end();  //Close connection
//    delay(1000000000000);
//TERMINA CODIGO VIEJITO
 }
}


//EMPIEZA CODIGO PARA POSTEAR UN JSON ARMADO
// StaticJsonDocument<300> doc;
//   doc["tipo"]=2;
//   doc["palabra"]="siuuuuu";
//   serializeJsonPretty(doc,Serial);
//   http.begin("http://httpbin.org/post");      //Specify request destination
//    http.addHeader("Content-Type", "application/json");  //Specify content-type header
//    String finalisima;
//    serializeJson(doc,finalisima);
//    int httpCode = http.POST(finalisima);   //Send the request
//    String payload = http.getString();                                        //Get the response payload
//
//    Serial.println(httpCode);   //Print HTTP return code
//    Serial.println(payload);    //Print request response payload
//
//    http.end();  //Close connection
// TERMINA EL CODIGO PARA POSTEAR EL JSON ARMADO
// }else{
//
//    Serial.println("Error in WiFi connection");
//
// }
//
//  delay(30000);  //Send a request every 30 seconds
//
//}
