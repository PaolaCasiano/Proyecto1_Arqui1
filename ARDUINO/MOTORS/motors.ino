#include <Servo.h>
#include <Stepper.h>

#define LINE_BUFFER_LENGTH 512

/*
   PARAMETRES DE CONFIGURATION
*/

int estadoImpresion; // Variable para controlar si esta en el estado de impresion<-------------------------------

const int penZUp = 20;                 // Angle servomoteur, crayon arriba
const int penZDown = 0;                            // Angle servomoteur, crayon abajo
const int penServoPin = 38;              // Pin sur lequel est relié le servomoteur
const int stepsPerRevolution = 20;              // Valeur par défaut
const int vitesseDeplacement = 250;             // Vitesse de déplacement des axes X et Y

Servo penServo;                     // Objet pour actionner le servomoteur

// Initialisation de los motores
Stepper myStepperY(stepsPerRevolution, 30,32,34,36);    // Axe Y         
Stepper myStepperX(stepsPerRevolution, 22,24,26,28);  // Axe X

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

//  Needs to interpret 
//  G1 for moving
//  G4 P300 (wait 150ms)
//  M300 S30 (pen down)
//  M300 S50 (pen up)
//  Discard anything with a (
//  Discard any other command!

/**********************
 * void setup() - Initialisations
 ***********************/
void setup() {
  estadoImpresion = 0;//Inicializar al valor o caracter designado para este metodo
  //  Setup
  Serial.begin( 9600 );
  
  penServo.attach(penServoPin);
  penServo.write(penZUp);
  delay(200);
   // Decrease if necessary
  myStepperX.setSpeed(vitesseDeplacement);
  myStepperY.setSpeed(vitesseDeplacement);  

}

/**********************
 * void loop() - Main loop
 ***********************/
void loop() 
{
  if(estadoImpresion == 0){
    interprete();
    estadoImpresion=1;
  }
}

void interprete(){
  delay(200);
  char line[ LINE_BUFFER_LENGTH ];
  char c;
  int lineIndex;
  bool lineIsComment, lineSemiColon;

  lineIndex = 0;
  lineSemiColon = false;
  lineIsComment = false;
  
  int salir = 0;
  while (salir == 0) {
    // Serial reception - Mostly from Grbl, added semicolon support
    while ( Serial.available()>0 ) {// Lee del WIFI<--------------------------------------------------------------------
      c = Serial.read();
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
          else if ( lineIndex >= LINE_BUFFER_LENGTH-1 ) {
            //Serial.println( "ERROR - lineBuffer overflow" );
            lineIsComment = false;
            lineSemiColon = false;
          } 
          else if ( c >= 'a' && c <= 'z' ) {        // Upcase lowercase
            line[ lineIndex++ ] = c-'a'+'A';
          }
          else if( c== '$'){
            //Serial.println("Saliendo");
            salir = 1;
            break;
          } 
          else {
            line[ lineIndex++ ] = c;
          }
        }
      }
    }
  }
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

  while( currentIndex < charNB ) {
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

      switch ( atoi( buffer ) ){                   // Select G command
      case 0:                                   // G00 & G01 - Movement or fast movement. Same here
      case 1:
        // /!\ Dirty - Suppose that X is before Y
        char* indexX = strchr( line+currentIndex, 'X' );  // Get X/Y position in the string (if any)
        char* indexY = strchr( line+currentIndex, 'Y' );
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
      switch ( atoi( buffer ) ){
      case 300:
        {
          char* indexS = strchr( line+currentIndex, 'S' );
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
  x1 = (int)(x1*StepsPerMillimeterX);
  y1 = (int)(y1*StepsPerMillimeterY);
  float x0 = Xpos;
  float y0 = Ypos;

  //  Let's find out the change for the coordinates
  long dx = abs(x1-x0);
  long dy = abs(y1-y0);
  int sx = x0<x1 ? StepInc : -StepInc;
  int sy = y0<y1 ? StepInc : -StepInc;

  long i;
  long over = 0;

  if (dx > dy) {
    for (i=0; i<dx; ++i) {
      myStepperX.step(sx);
      over+=dy;
      if (over>=dx) {
        over-=dx;
        myStepperY.step(sy);
      }
      delay(StepDelay);
    }
  }
  else {
    for (i=0; i<dy; ++i) {
      myStepperY.step(sy);
      over+=dx;
      if (over>=dy) {
        over-=dy;
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
  Zpos=Zmax; 
  if (verbose) { 
    //Serial.println("Pen up!"); 
  } 
}

//  Lowers pen
void penDown() { 
  penServo.write(penZDown); 
  delay(LineDelay); 
  Zpos=Zmin; 
  if (verbose) { 
    //Serial.println("Pen down."); 
  } 
}