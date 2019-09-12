// Paint example specifically for the TFTLCD breakout board.
// If using the Arduino shield, use the tftpaint_shield.pde sketch instead!
// DOES NOT CURRENTLY WORK ON ARDUINO LEONARDO

// Modified for SPFD5408 Library by Joao Lopes
// Version 0.9.2 - Rotation for Mega

// *** SPFD5408 change -- Begin
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>

#include<Arduino.h>
#include<SoftwareSerial.h> 
// *** SPFD5408 change -- End

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 22
//   D1 connects to digital pin 23
//   D2 connects to digital pin 24
//   D3 connects to digital pin 25
//   D4 connects to digital pin 26
//   D5 connects to digital pin 27
//   D6 connects to digital pin 28
//   D7 connects to digital pin 29

// For the Arduino Due, use digital pins 33 through 40
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 33
//   D1 connects to digital pin 34
//   D2 connects to digital pin 35
//   D3 connects to digital pin 36
//   D4 connects to digital pin 37
//   D5 connects to digital pin 38
//   D6 connects to digital pin 39
//   D7 connects to digital pin 40

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin


// Original values
//#define TS_MINX 150
//#define TS_MINY 120
//#define TS_MAXX 920
//#define TS_MAXY 940

// Calibrate values
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 2


SoftwareSerial myserial(19,18);
using namespace std;
int oldcolor, currentcolor;

//VARIABLES PARA MANDAR LA IMAGEN
String coordenadas;
String cadena;
void setup(void) {
  
    //Serial.begin(4800);//Serial connection
    //Serial.begin(9600);//Serial connection
    myserial.begin(9600);
    //Serial.println(F("Paint!"));
  
    tft.reset();

  // *** SPFD5408 change -- Begin
//  uint16_t identifier = tft.readID();
//
//  if(identifier == 0x9325) {
//    Serial.println(F("Found ILI9325 LCD driver"));
//  } else if(identifier == 0x9328) {
//    Serial.println(F("Found ILI9328 LCD driver"));
//  } else if(identifier == 0x7575) {
//    Serial.println(F("Found HX8347G LCD driver"));
//  } else if(identifier == 0x9341) {
//    Serial.println(F("Found ILI9341 LCD driver"));
//  } else if(identifier == 0x8357) {
//    Serial.println(F("Found HX8357D LCD driver"));
//  } else {
//    Serial.print(F("Unknown LCD driver chip: "));
//    Serial.println(identifier, HEX);
//    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
//    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
//    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
//    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
//    Serial.println(F("Also if using the breakout, double-check that all wiring"));
//    Serial.println(F("matches the tutorial."));
//    return;
//  }
//
//  tft.begin(identifier);

  tft.begin(0x9341); // SDFP5408

  tft.setRotation(0); // Need for the Mega, please changed for your choice or rotation initial

  
  tft.fillScreen(WHITE);
tft.setTextColor(BLACK);
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, BLACK);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, BLUE);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, RED);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE*3, BOXSIZE, YELLOW);
   tft.setTextSize(2);
   tft.setCursor(BOXSIZE*3+15, 15);
   tft.println("IMPRIMIR");
   //NOMBRES
   tft.setTextSize(1.5);
   tft.setCursor(0, BOXSIZE+3);
   tft.println("Max Florian - 201700465");
   tft.setCursor(0, BOXSIZE+11);
   tft.println("Luis Ralda - 201709065");
   tft.setCursor(0, BOXSIZE+19);
   tft.println("Paola Casiano - 201503595");
   tft.setCursor(0, BOXSIZE+25);
   tft.println("Cristian Castellanos - 201709177");

 tft.drawRect(0, 0, BOXSIZE, BOXSIZE, MAGENTA);
  currentcolor = BLACK;
 
  pinMode(13, OUTPUT);
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    /*
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
    */
    /*
    if (p.y < (TS_MINY-5)) {
      Serial.println("erase");
      // press the bottom of the screen to erase 
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
    }
    */
    // scale from 0->1023 to tft.width

    // *** SPFD5408 change -- Begin
    // Bug in in original code
    //p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, tft.width());
    // *** SPFD5408 change -- End
    p.y = map(p.y-50, TS_MINY, TS_MAXY, 0, tft.height());

    /*
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");
    */
    if (p.y < BOXSIZE) {
       oldcolor = currentcolor;

       if (p.x < BOXSIZE) { 
         currentcolor = BLACK; 
         tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*2) {
         currentcolor = BLUE;
         tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*3) {
         currentcolor = RED;
         tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, WHITE);
       }

       if (oldcolor != currentcolor) {
          if (oldcolor == BLACK) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, BLACK);
          if (oldcolor == BLUE) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, BLUE);
          if (oldcolor == RED) tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, RED);
       }
    //PRESIONAR EL BOTON DE IMPRIMIR
    if(p.x > BOXSIZE*3 && p.x<tft.width()){
      //Hacer la peticion al web service
      //Serial.println(F("SE PRESIONO IMPRIMIR"));
      //Serial.println(cadena);
      PostReq(cadena);
      cadena=="";
      }
       
    }
    if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
      tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
      cadena = cadena  + p.x+","+p.y+";";
    }
  }
}

void PostReq(String coordenadas){

    coordenadas = "&"+coordenadas+"&";
    int str_len = coordenadas.length() + 1; 
    char char_array[str_len];
     
    // Copy it over 
    
    coordenadas.toCharArray(char_array, str_len);
    //myserial.write(char_array);
    myserial.print(coordenadas);
    coordenadas = "";
   /*if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin("http://192.168.1.88:8085/hello");      //Specify request destination
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
 
   int httpCode = http.POST(cadena);   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }*/
  }
