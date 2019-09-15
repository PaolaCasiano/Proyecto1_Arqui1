
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#include<Arduino.h>
#include<SoftwareSerial.h>
#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin


// Calibrate values
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905

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

using namespace std;
int oldcolor, currentcolor;

//VARIABLES PARA MANDAR LA IMAGEN
String post;
String cadena;



#define MINPRESSURE 10
#define MAXPRESSURE 1000

//Para motores

#include <Servo.h>
#include <Stepper.h>

#define LINE_BUFFER_LENGTH 512

int estadoImpresion; // Variable para controlar si esta en el estado de impresion<-------------------------------

const int penZUp = 20;                 // Angle servomoteur, crayon arriba
const int penZDown = 0;                            // Angle servomoteur, crayon abajo
const int penServoPin = 38;              // Pin sur lequel est relié le servomoteur
const int stepsPerRevolution = 20;              // Valeur par défaut
const int vitesseDeplacement = 250;             // Vitesse de déplacement des axes X et Y

Servo penServo;                     // Objet pour actionner le servomoteur

// Initialisation de los motores
Stepper myStepperY(stepsPerRevolution, 30, 32, 34, 36); // Axe Y
Stepper myStepperX(stepsPerRevolution, 22, 24, 26, 28); // Axe X

// Calibration, nombre de pas par millimètre
float StepsPerMillimeterX = 6.0;
float StepsPerMillimeterY = 6.0;

/*
    FIN DE LA CONFIGURATION
*/

/* Structures, global variables    */
struct point {
  float x;
  float y;
  float z;
};

// Current position of plothead
struct point actuatorPos;

//  Drawing settings, should be OK
float StepInc = 1;
int StepDelay = 0;
int LineDelay = 50;
int penDelay = 50;

// Drawing robot limits, in mm
// OK to start with. Could go up to 50 mm if calibrated well.
float Xmin = 0;
float Xmax = 40;
float Ymin = 0;
float Ymax = 40;
float Zmin = 0;
float Zmax = 1;

float Xpos = Xmin;
float Ypos = Ymin;
float Zpos = Zmax;

// Set to true to get debug output.
boolean verbose = false;
char c;
int lineIndex;
bool lineIsComment, lineSemiColon;
char line[ LINE_BUFFER_LENGTH ];



void setup(void) {

  estadoImpresion = 0;//Inicializar al valor o caracter designado para este metodo
  //  Setup

  penServo.attach(penServoPin);
  penServo.write(penZUp);
  delay(200);
  // Decrease if necessary
  myStepperX.setSpeed(vitesseDeplacement);
  myStepperY.setSpeed(vitesseDeplacement);

  cadena = "";

  Serial.begin(9600);//Serial connection
  Serial1.begin(9600);
  //myserial.begin(9600);

  tft.reset();
  tft.begin(0x9341); // SDFP5408

  tft.setRotation(0); // Need for the Mega, please changed for your choice or rotation initial


  tft.fillScreen(WHITE);
  tft.setTextColor(BLACK);
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, BLACK);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, BLUE);
  tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, RED);
  tft.fillRect(BOXSIZE * 3, 0, BOXSIZE * 3, BOXSIZE, YELLOW);
  tft.setTextSize(2);
  tft.setCursor(BOXSIZE * 3 + 15, 15);
  tft.println("IMPRIMIR");
  //NOMBRES
  tft.setTextSize(1.5);
  tft.setCursor(0, BOXSIZE + 3);
  tft.println("Max Florian - 201700465");
  tft.setCursor(0, BOXSIZE + 11);
  tft.println("Luis Ralda - 201709065");
  tft.setCursor(0, BOXSIZE + 19);
  tft.println("Paola Casiano - 201503595");
  tft.setCursor(0, BOXSIZE + 25);
  tft.println("Cristian Castellanos - 201709177");

  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, MAGENTA);
  currentcolor = BLACK;

  pinMode(13, OUTPUT);
}
void loop()
{

  //Serial.println(Serial1.available());
  if (Serial1.available() > 0) {
    post = Serial1.readString();
    Serial.println(post);
    for (int i = 0; i < post.length(); ++i)
    {
      interprete(post.charAt(i));
    }
    //interprete(Serial1.read());
    //Serial.println(Serial1.readString());
  } else {
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
      // scale from 0->1023 to tft.width

      // *** SPFD5408 change -- Begin
      // Bug in in original code
      //p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
      p.x = map(p.x, TS_MAXX, TS_MINX, 0, tft.width());
      // *** SPFD5408 change -- End
      p.y = map(p.y - 50, TS_MINY, TS_MAXY, 0, tft.height());

      if (p.y < BOXSIZE) {
        oldcolor = currentcolor;

        if (p.x < BOXSIZE) {
          currentcolor = BLACK;
          tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
        } else if (p.x < BOXSIZE * 2) {
          currentcolor = BLUE;
          tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
        } else if (p.x < BOXSIZE * 3) {
          currentcolor = RED;
          tft.drawRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, WHITE);
        }

        if (oldcolor != currentcolor) {
          if (oldcolor == BLACK) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, BLACK);
          if (oldcolor == BLUE) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, BLUE);
          if (oldcolor == RED) tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, RED);
        }
        //PRESIONAR EL BOTON DE IMPRIMIR
        if (p.x > BOXSIZE * 3 && p.x < tft.width()) {
          //Hacer la peticion al web service
          Serial.println(F("SE PRESIONO IMPRIMIR"));
          PostReq(cadena);

        }

      }
      if (((p.y - PENRADIUS) > BOXSIZE) && ((p.y + PENRADIUS) < tft.height())) {
        tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
        cadena = cadena  + p.x + "," + p.y + ";";
      }
    }
  }
}

void PostReq(String coordenadas) {

  int str_len = coordenadas.length() + 1;
  char char_array[str_len];

  coordenadas.toCharArray(char_array, str_len);
  Serial1.println(coordenadas);
  //myserial.write(char_array);
  Serial.print(coordenadas);
  //coordenadas = "";
}



void interprete(char c) {
  
  

  lineIndex = 0;
  lineSemiColon = false;
  lineIsComment = false;

  //int salir = 0;
  //while (salir == 0) {
    // Serial reception - Mostly from Grbl, added semicolon support
    //while ( Serial.available() > 0 ) { // Lee del WIFI<--------------------------------------------------------------------
      //c = Serial1.read();
      //c = 's';
      //Serial.println(c);
      if (( c == '\n') || (c == '\r') ) {             // End of line reached
        if ( lineIndex > 0 ) {                        // Line is complete. Then execute!
          line[ lineIndex ] = '\0';                   // Terminate string
          if (verbose) {
            //Serial.print( "Received : ");
            //Serial.println( line );
          }
          processIncomingLine( line, lineIndex );//La linea se manda analizar<---------------------------------------------
          lineIndex = 0;
        }
        else {
          // Empty or comment line. Skip block.
        }
        lineIsComment = false;
        lineSemiColon = false;
        //Serial.println("ok");
      }
      else {
        if ( (lineIsComment) || (lineSemiColon) ) {   // Throw away all comment characters
          if ( c == ')' )  lineIsComment = false;     // End of comment. Resume line.
        }
        else {
          if ( c <= ' ' ) {                           // Throw away whitepace and control characters
          }
          else if ( c == '/' ) {                    // Block delete not supported. Ignore character.
          }
          else if ( c == '(' ) {                    // Enable comments flag and ignore all characters until ')' or EOL.
            lineIsComment = true;
          }
          else if ( c == ';' ) {
            lineSemiColon = true;
          }
          else if ( lineIndex >= LINE_BUFFER_LENGTH - 1 ) {
            //Serial.println( "ERROR - lineBuffer overflow" );
            lineIsComment = false;
            lineSemiColon = false;
          }
          else if ( c >= 'a' && c <= 'z' ) {        // Upcase lowercase
            line[ lineIndex++ ] = c - 'a' + 'A';
          }
          else if ( c == '$') {
            //Serial.println("Saliendo");
            //salir = 1;
            return;
          }
          else {
            line[ lineIndex++ ] = c;
          }
        }
      }
      delay(1);
    //}
  //}
}

void processIncomingLine( char* line, int charNB ) {
  //Serial.println("Procesando");
  int currentIndex = 0;
  char buffer[ 64 ];                                 // Hope that 64 is enough for 1 parameter
  struct point newPos;

  newPos.x = 0.0;
  newPos.y = 0.0;

  //  Needs to interpret
  //  G1 for moving
  //  G4 P300 (wait 150ms)
  //  G1 X60 Y30
  //  G1 X30 Y50
  //  M300 S30 (pen down)
  //  M300 S50 (pen up)
  //  Discard anything with a (
  //  Discard any other command!

  while ( currentIndex < charNB ) {
    switch ( line[ currentIndex++ ] ) {              // Select command, if any
      case 'U':
        penUp();
        break;
      case 'D':
        penDown();
        break;
      case 'G':
        buffer[0] = line[ currentIndex++ ];          // /!\ Dirty - Only works with 2 digit commands
        //      buffer[1] = line[ currentIndex++ ];
        //      buffer[2] = '\0';
        buffer[1] = '\0';

        switch ( atoi( buffer ) ) {                  // Select G command
          case 0:                                   // G00 & G01 - Movement or fast movement. Same here
          case 1:
            // /!\ Dirty - Suppose that X is before Y
            char* indexX = strchr( line + currentIndex, 'X' ); // Get X/Y position in the string (if any)
            char* indexY = strchr( line + currentIndex, 'Y' );
            if ( indexY <= 0 ) {
              newPos.x = atof( indexX + 1);
              newPos.y = actuatorPos.y;
            }
            else if ( indexX <= 0 ) {
              newPos.y = atof( indexY + 1);
              newPos.x = actuatorPos.x;
            }
            else {
              newPos.y = atof( indexY + 1);
              indexY = '\0';
              newPos.x = atof( indexX + 1);
            }
            drawLine(newPos.x, newPos.y );
            //        Serial.println("ok");
            actuatorPos.x = newPos.x;
            actuatorPos.y = newPos.y;
            break;
        }
        break;
      case 'M':
        buffer[0] = line[ currentIndex++ ];        // /!\ Dirty - Only works with 3 digit commands
        buffer[1] = line[ currentIndex++ ];
        buffer[2] = line[ currentIndex++ ];
        buffer[3] = '\0';
        switch ( atoi( buffer ) ) {
          case 300:
            {
              char* indexS = strchr( line + currentIndex, 'S' );
              float Spos = atof( indexS + 1);
              //          Serial.println("ok");
              if (Spos == 30) {
                penDown();
              }
              if (Spos == 50) {
                penUp();
              }
              break;
            }
          case 114:                                // M114 - Repport position
            //Serial.print( "Absolute position : X = " );
            //Serial.print( actuatorPos.x );
            //Serial.print( "  -  Y = " );
            //Serial.println( actuatorPos.y );
            break;
          default:
            //Serial.print( "Command not recognized : M");
            //Serial.println( buffer );
            break;
        }
    }
  }
}

void drawLine(float x1, float y1) {

  if (verbose)
  {
    //Serial.print("fx1, fy1: ");
    //Serial.print(x1);
    //Serial.print(",");
    //Serial.print(y1);
    //Serial.println("");
  }

  //  Bring instructions within limits
  if (x1 >= Xmax) {
    x1 = Xmax;
  }
  if (x1 <= Xmin) {
    x1 = Xmin;
  }
  if (y1 >= Ymax) {
    y1 = Ymax;
  }
  if (y1 <= Ymin) {
    y1 = Ymin;
  }

  if (verbose)
  {
    //Serial.print("Xpos, Ypos: ");
    //Serial.print(Xpos);
    //Serial.print(",");
    //Serial.print(Ypos);
    //Serial.println("");
  }

  if (verbose)
  {
    //Serial.print("x1, y1: ");
    //Serial.print(x1);
    //Serial.print(",");
    //Serial.print(y1);
    //Serial.println("");
  }

  //  Convert coordinates to steps
  x1 = (int)(x1 * StepsPerMillimeterX);
  y1 = (int)(y1 * StepsPerMillimeterY);
  float x0 = Xpos;
  float y0 = Ypos;

  //  Let's find out the change for the coordinates
  long dx = abs(x1 - x0);
  long dy = abs(y1 - y0);
  int sx = x0 < x1 ? StepInc : -StepInc;
  int sy = y0 < y1 ? StepInc : -StepInc;

  long i;
  long over = 0;

  if (dx > dy) {
    for (i = 0; i < dx; ++i) {
      myStepperX.step(sx);
      over += dy;
      if (over >= dx) {
        over -= dx;
        myStepperY.step(sy);
      }
      delay(StepDelay);
    }
  }
  else {
    for (i = 0; i < dy; ++i) {
      myStepperY.step(sy);
      over += dx;
      if (over >= dy) {
        over -= dy;
        myStepperX.step(sx);
      }
      delay(StepDelay);
    }
  }

  if (verbose)
  {
    //Serial.print("dx, dy:");
    //Serial.print(dx);
    //Serial.print(",");
    //Serial.print(dy);
    //Serial.println("");
  }

  if (verbose)
  {
    //Serial.print("Going to (");
    //Serial.print(x0);
    //Serial.print(",");
    //Serial.print(y0);
    //Serial.println(")");
  }

  //  Delay before any next lines are submitted
  delay(LineDelay);
  //  Update the positions
  Xpos = x1;
  Ypos = y1;
}

//  Raises pen
void penUp() {
  penServo.write(penZUp);
  delay(LineDelay);
  Zpos = Zmax;
  if (verbose) {
    //Serial.println("Pen up!");
  }
}

//  Lowers pen
void penDown() {
  penServo.write(penZDown);
  delay(LineDelay);
  Zpos = Zmin;
  if (verbose) {
    //Serial.println("Pen down.");
  }
}
